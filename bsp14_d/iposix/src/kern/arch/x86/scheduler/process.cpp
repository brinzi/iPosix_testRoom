#include "process.h"
#include <algorithm>
#include "config/memory.h"
#include "mm/kmalloc.h"
#include "die.h"
#include "logger/logger.h"

namespace iposix {
namespace arch {
namespace x86 {

using hlmemory::vmalloc;
using hlmemory::kfree;

page_dir_entry * process::direct_mapping( 0 );

process::process( bool kernel )
	:	ptable( vm::create_page_directory_table() ),
	ptable_end( ptable + 1024 )
{
	scoped_paging_off paging_lock( true );

	if( !process::direct_mapping )
	{
		process::direct_mapping = vm::create_page_directory_table();
		virtual_address v( 0 );
		for( page_dir_entry * it( process::direct_mapping ); it < process::direct_mapping + 1024; ++it, ++v.divided.dir )
		{
			it->flat = v.normal;
			it->valid.presence = true;
			it->valid.write = true;
			it->valid.user_supervisor = false;
			it->valid.cache_disable = false;
			it->valid.write_through = true;
			it->valid.access = false;
			it->valid.page_size = true;
			it->valid.global = true;
			it->valid.direct_mapped = true;
		}
	}

	{
		//jeder prozess hat 128mb kernel memory direct mapped, aber nur mit cpl=0 zugreifbar

		for( uint_fast8_t i = 0; i < 32; ++i )
		{
			this->ptable[i] = process::direct_mapping[i];
		}
	}
}

process::~process()
{
	//memory schould be freed
	delete[] this->ptable;
}

void *
process::add_virtual_memory( const vector< void * >& addresses, virtual_address vaddr )
{
	scoped_paging_off paging_lock( true );

	auto count = addresses.size();

	if( count == 0 )
	{
		return 0;
	}

	vector< page_table_entry * > used;
	used.reserve( count );
	virtual_address v( 0 );

	if( vaddr.normal == 0 )
	{
		v.divided.dir = 512;
		for( page_dir_entry * it = this->ptable + v.divided.dir; it < this->ptable_end; ++it )
		{
			if( !it->invalid.presence )
			{
				it->flat = vm::create_page_table();
				it->valid.presence = true;
				it->valid.write = true;
				it->valid.user_supervisor = true;
				it->valid.cache_disable = false;
				it->valid.write_through = true;
				it->valid.access = false;
				it->valid.page_size = false;
				it->valid.global = false;
				it->valid.direct_mapped = false;
			}
			else if( it->valid.page_size ) //4MiB pages
			{
				return 0;
			}

			page_table_entry * pte = reinterpret_cast< page_table_entry * >( it->address() );

			for( page_table_entry * it2 = pte; it2 < ( pte + 1024 ); ++it2 )
			{
				if( it2->invalid.presence )
				{
					used.clear();
					count = addresses.size();
				}
				else
				{
					if( count == addresses.size() )
					{
						v.divided.page = it2 - pte;
					}
					used.push_back( it2 );
					--count;
				}

				if( count == 0 )
				{
					break;
				}
			}

			if( count == 0 )
			{
				break;
			}

			++v.divided.dir;
		}

		if( count != 0 )
		{
			return 0;
		}
	}
	else
	{
		bool breakout = false;

		for( page_dir_entry * it = this->ptable + vaddr.divided.dir; it < this->ptable_end; ++it )
		{
			if( !it->invalid.presence )
			{
				it->flat = vm::create_page_table();
				it->valid.presence = true;
				it->valid.write = true;
				it->valid.user_supervisor = true;
				it->valid.cache_disable = false;
				it->valid.write_through = true;
				it->valid.access = false;
				it->valid.page_size = false;
				it->valid.global = false;
				it->valid.direct_mapped = false;
			}
			else if( it->valid.page_size )
			{
				return 0;
			}

			page_table_entry * pte = reinterpret_cast< page_table_entry * >( it->address() );
			page_table_entry * it2 = pte;

			if( it == this->ptable + vaddr.divided.dir )
			{
				it2 += vaddr.divided.page;
			}
			
			for( ; it2 < ( pte + 1024 ); ++it2 )
			{
				if( it2->invalid.presence )
				{
					return 0;
				}
				else
				{
					used.push_back( it2 );
					--count;

					if( count == 0 )
					{
						breakout = true;
					}
				}

				if( breakout )
					break;
			}

			if( breakout )
				break;
		}

		v = vaddr;
	}

	const auto vit_end = used.end();
	auto va = addresses.begin();
	for( auto vit = used.begin(); vit != vit_end; ++vit )
	{
		page_table_entry * pte = *vit;

		pte->valid.presence = true;
		pte->valid.write = true;
		pte->valid.user_supervisor = true;
		pte->valid.write_through = true;
		pte->valid.cache_disable = false;
		pte->valid.access = false;
		pte->valid.dirty = false;
		pte->valid.page_table_attribute_index = false;
		pte->valid.global = false;
		pte->valid.direct_mapped = false;
		pte->valid.page_frame_address = reinterpret_cast< uintptr_t >( *va ) >> 12;
		++va;
	}

	return v.normal;
}

void *
process::get_phys_address( virtual_address vaddr, page_table_entry * entry_out ) const
{
	scoped_paging_off paging_lock( true );

	page_dir_entry& dentry = this->ptable[vaddr.divided.dir];

	if( !dentry.valid.presence )
	{
		return 0;
	}

	if( dentry.valid.page_size ) //4MiB pages
	{
		if( entry_out )
		{
			entry_out->flat = dentry.flat;
		}

		auto phys = reinterpret_cast< uintptr_t >( dentry.flat ) & 0xFFC00000;
		phys |= ( reinterpret_cast< uintptr_t >( vaddr.normal ) & 0x003FFFFF );

		return reinterpret_cast< void * >( phys );
	}

	page_table_entry * entry = reinterpret_cast< page_table_entry * >( dentry.address() );

	vm::page_table_entry& e = entry[vaddr.divided.page];

	if( entry_out )
	{
		*entry_out = e;
	}

	if( !e.valid.presence )
	{
		return 0;
	}

	auto phys = reinterpret_cast< uintptr_t >( e.flat ) & 0xFFFFF000;
	phys |= vaddr.divided.offset;

	return reinterpret_cast< void * >( phys );
}

bool
process::remove_virtual_memory( virtual_address vaddr )
{
	scoped_paging_off paging_lock( true );

	page_dir_entry * pde = &this->ptable[vaddr.divided.dir];

	if( !pde->valid.presence )
	{
		return false;
	}
	else if( pde->valid.page_size )
	{
		return false; //only kernel pages are 4MiB an cannot be removed
	}

	page_table_entry * pte = reinterpret_cast< page_table_entry * >( pde->address() ) + vaddr.divided.page;

	if( !pte->valid.presence )
	{
		return false;
	}

	pte->invalid.presence = false;

	return true;
}


void
process::unpresent( virtual_address /*vaddr*/ )
{
	scoped_paging_off paging_lock( true );

}

void
process::present( virtual_address /*vaddr*/, void * /*addr*/ )
{
	scoped_paging_off paging_lock( true );

}

bool
process::copy_in( virtual_address vaddr, void * addr, size_t size, bool copyout )
{
	scoped_paging_off paging_lock( true );

	if( size == 0 )
	{
		return true;
	}

	if( vaddr.normal == 0 || addr == 0 )
	{
		return false;
	}

	//start liegt vielleicht nicht an seitengrenze
	if( vaddr.divided.offset )
	{
		size_t csize = process::page_size - vaddr.divided.offset; //rest in der angefangenen seite
		csize = size < csize ? size : csize; //eventuell muss weniger kopiert werden als der rest

		uint8_t * dptr = reinterpret_cast< uint8_t * >( this->get_phys_address( vaddr ) );
		if( !dptr )
		{
			return false;
		}
		uint8_t * sptr = reinterpret_cast< uint8_t * >( addr );

		if( copyout )
		{
			std::swap( dptr, sptr );
		}

		for( size_t i = 0; i < csize; ++i )
		{
			*(dptr++) = *(sptr++);
		}

		size -= csize;
		if( size == 0 )
		{
			return true;
		}

		vaddr.divided.offset = 0;
		++vaddr.divided.page;
		if( vaddr.divided.page == 0 ) //ueberlauf
		{
			++vaddr.divided.dir;
			if( vaddr.divided.dir == 0 ) //ueberlauf -> fehler
			{
				return false;
			}
		}

		if( copyout )
		{
			sptr = dptr;
		}

		addr = reinterpret_cast< void * >( sptr );
	}

	//ganze seiten
	if( size >= process::page_size )
	{
		size_t count = size / process::page_size;
		
		struct __attribute__((packed)) helper
		{
			uint8_t help[process::page_size];
		};

		helper * src = reinterpret_cast< helper * >( addr );
		for( size_t i = 0; i < count; ++i )
		{
			helper * dest = reinterpret_cast< helper * >( this->get_phys_address( vaddr ) );
			if( dest == 0 )
			{
				return false;
			}

			if( copyout )
			{
				*(src++) = *dest;
			}
			else
			{
				*dest = *(src++);
			}

			++vaddr.divided.page;
			if( vaddr.divided.page == 0 )
			{
				++vaddr.divided.dir;
				if( vaddr.divided.dir == 0 )
				{
					return false;
				}
			}
		}

		size -= process::page_size * count;

		if( size == 0 )
		{
			return true;
		}

		addr = reinterpret_cast< void * >( src );
	}

	uint8_t * dest = reinterpret_cast< uint8_t * >( this->get_phys_address( vaddr ) );
	if( dest == 0 )
	{
		return false;
	}
	uint8_t * src = reinterpret_cast< uint8_t * >( addr );

	if( copyout )
	{
		std::swap( src, dest );
	}

	while( size > 0 )
	{
		*(dest++) = *(src++);
		--size;
	}
	
	return true;
}


bool
process::copy_address_space( const process& proc )
{
	scoped_paging_off paging_lock( true );

	virtual_address v( 0 );

	for( page_dir_entry * it_proc( proc.ptable ); it_proc < proc.ptable_end; ++it_proc, ++v.divided.dir )
	{
		if ( it_proc->valid.presence && it_proc->valid.direct_mapped )
		{
			//copy direct mapped entrys
			this->ptable[v.divided.dir] = *it_proc;
		}
		else if( it_proc->valid.presence )
		{
			v.divided.page = 0;

			page_table_entry * pte = reinterpret_cast< page_table_entry * >( it_proc->address() );
			page_table_entry * pteend = pte + 1024;

			for( page_table_entry * proc_entry( pte ); proc_entry < pteend; ++proc_entry, ++v.divided.page )
			{
				if( proc_entry->valid.presence && !proc_entry->valid.direct_mapped )
				{
					void * alloc = vmalloc( process::page_size, *this, v.normal );

					if( alloc != v.normal )
					{
						logger << message::MORE_VERBOSE << __func__
							<< ": Something went horribly wrong when copying an address space." << endl;

						//TODO aufraeumen

						return false;
					}

					struct helper
					{
						uint8_t helper_array[process::page_size];
					};

					helper * phys1 = reinterpret_cast< helper * >( this->get_phys_address( alloc ) );
					helper * phys2 = reinterpret_cast< helper * >( proc.get_phys_address( v ) );

					*phys1 = *phys2;
				}
			}
		}
	}

	return true;
}

bool process::prepare_running()
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Setting Pagetable to "
		<< hex << reinterpret_cast<uintptr_t>(this->ptable) << dec << endl;

	if ( this->ptable != 0 )
	{
		::iposix::utils::Singleton< MEMORY >::instance().set_page_table( reinterpret_cast< void * >( this->page_table_ptr() ) );
	}

	return true;
}

uintptr_t process::page_table_ptr()
{
	uintptr_t ptr = reinterpret_cast< uintptr_t >( this->ptable );

	if( ( ptr & 0xFFF ) != 0 )
	{
		logger << message::VERBOSE << "table not aligned." << endl;
		die();
	}

	//return ( ptr | 0x10 ); //Setzte Write-Back und Page Cache Enable
	return ( ptr );
}

bool process::reset_address_space()
{
	scoped_paging_off paging_lock( true );

	virtual_address v( 0 );
	bool success = true;

	for( page_dir_entry * it( this->ptable ); it < this->ptable_end; ++it, ++v.divided.dir )
	{
		if( it->valid.presence && !it->valid.direct_mapped )
		{
			v.divided.page = 0;

			page_table_entry * pte = reinterpret_cast< page_table_entry * >( it->address() );
			page_table_entry * pteend = pte + 1024;

			for( page_table_entry * proc_entry( pte ); proc_entry < pteend; ++proc_entry, ++v.divided.page )
			{
				if( proc_entry->valid.presence && !proc_entry->valid.direct_mapped )
				{
					success &= kfree( v.normal, false );
				}
			}
		}
	}

	if( !success )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": error during reseting address space" << endl;
	}

	return success;
	
}

void
process::dump_vm()
{
	scoped_paging_off paging_lock( true );

	logger << message::MORE_VERBOSE;

	logger << "Dump of the VM (virtual address -> physical address):" << hex << endl;

	page_table_entry entry;

	for( virtual_address v( reinterpret_cast< void * >( 4096 ) ); v.normal != 0; v.normal = reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( v.normal ) + 4096 ) )
	{
		uintptr_t mapped_to = reinterpret_cast< uintptr_t >( this->get_phys_address( v, &entry ) );

		if( entry.valid.presence )
		{
			logger << reinterpret_cast< uintptr_t >( v.normal ) << " -> " << mapped_to << " flags: ";
			char flags[6] = "-S---";
			if( entry.valid.write ) flags[0] = 'W';
			if( entry.valid.user_supervisor ) flags[1] = 'U';
			if( entry.valid.access ) flags[2] = 'A';
			if( entry.valid.dirty ) flags[3] = 'D';
			if( entry.valid.direct_mapped ) flags[4] = 'P';
			logger << flags << endl;
		}
	}

	logger << "End of VM Dump" << endl;
}

}}} //iposix::arch::x86
