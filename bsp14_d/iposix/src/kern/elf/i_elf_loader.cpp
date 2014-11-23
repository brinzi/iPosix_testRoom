#include "elf/i_elf_loader.h"
#include "elf/elf.h"
#include "fs/i_fs.h"
#include "fs/i_fs_node.h"
#include "logger/logger.h"
#include "scheduler/i_process.h"
#include "mm/kmalloc.h"

namespace iposix
{
namespace elf
{

using ::iposix::fs::i_fs;
using ::iposix::scheduler::process_type;
using ::iposix::scheduler::current_process;
using ::iposix::hlmemory::vmalloc;

template<>
elf_loader_type::i_elf_loader()
{ }

template<>
elf_loader_type::~i_elf_loader()
{ }

template<>
bool elf_loader_type::allocate_area(
		process_type& process,
		uintptr_t vaddr,
		uint64_t size,
		uint64_t align,
		bool read,
		bool write,
		bool execute
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Should allocate area at " << hex << vaddr
		<< " size " << dec << size
		<< " align " << dec << align
		<< " flags "
		<< ( read ? "R" : "-" )
		<< ( write ? "W" : "-" )
		<< ( execute ? "X" : "-" )
		<< endl;

	uint64_t area_end( vaddr + size );
	if ( reinterpret_cast<uintptr_t>(process.heap_top) < area_end )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Setting heap_top to " << hex << area_end << dec << endl;

		process.heap_top = reinterpret_cast<void*>( area_end );
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Access rights are ignored" << endl;

	//allocate virual memory
	uintptr_t ret = 
		reinterpret_cast<uintptr_t>(
				vmalloc(
					static_cast<uint32_t>( size ),
					process,
					reinterpret_cast<void*>( vaddr )
					)
				);
	
	bool success = ( vaddr == ret );

	if ( !success )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Allocation failed" << endl;
	}

	return success;
}

template<>
bool elf_loader_type::allocate_stack(
		process_type& process
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Should allocate stack at " << hex << INITIAL_STACK_BASE
		<< " size " << dec << INITIAL_ELF_STACKSIZE
		<< " align " << dec << STACK_ALIGNMENT
		<< " flags RW-"
		<< endl;

	logger << message::MORE_VERBOSE << __func__
		<< ": Access rights are ignored" << endl;

	//allocate virual memory
	uintptr_t ret =
		reinterpret_cast<uintptr_t>(
				vmalloc(
					INITIAL_ELF_STACKSIZE,
					process,
					reinterpret_cast<void*>( INITIAL_STACK_BASE )
					)
				);
	
	return ( ret == INITIAL_STACK_BASE );
}

template<>
bool elf_loader_type::copy_to_area(
		process_type& process,
		i_fs_node_ptr file,
		uint64_t file_offset,
		uintptr_t vaddr,
		uint64_t size
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Copying data from file " << file->path.c_str()
		<< " at pos " << hex << file_offset
		<< " to vaddr " << hex << vaddr
		<< " with size " << dec << size
		<< endl;

	uint8_t* buffer = new uint8_t[size];

	file->read( file_offset, reinterpret_cast<void*>(buffer), size );

	process.copy_in(
			reinterpret_cast<void*>( vaddr ),
			&buffer[0],
			size );
	
	delete[] buffer;

	return true;
}

template<>
bool elf_loader_type::load_elf(
		const ::std::string& file_name,
		char* const args[]
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Loading" << file_name.c_str() << endl;
	
	process_type* cur_process = current_process();
	if ( cur_process == 0 )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": No current process. Aborting..." << endl;
		return false;
	}
	process_type& process = *cur_process;

	i_fs_node_ptr file = i_fs::open_node( file_name );

	if ( !file )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": No such file" << endl;
		return false;
	}

	// Load the ELF header
	Elf32_Ehdr elf_header;
	file->read( 0, &elf_header, sizeof( Elf32_Ehdr ) );

	for ( uint32_t i = 0; i < 4; ++i )
	{
		if ( elf_header.e_ident[i] != ELFMAG[i] )
		{
			logger << message::MORE_VERBOSE << __func__
				<< ": Not an ELF file" << endl;
			return false;
		}
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Reseting address space" << endl;
	
	process.reset_address_space();
	process.heap_top = 0;

	logger << message::MORE_VERBOSE << __func__
		<< ": Program Header Offset" << elf_header.e_phoff << endl;

	logger << message::MORE_VERBOSE << __func__
		<< ": Allocating Program Segments" << endl;

	// Load the Program segment describtions
	// NOTE we could load all segments together
	uint64_t offset( elf_header.e_phoff );
	bool success = true;
	for (
			uint32_t i = 0;
			i < elf_header.e_phnum;
			++i, offset += sizeof( Elf32_Phdr)
		)
	{
		Elf32_Phdr p_hdr;
		file->read( offset, &p_hdr, sizeof( Elf32_Phdr ) );

		logger << message::MORE_VERBOSE << __func__
			<< ": Segment " << dec << i
			<< " with type "<< hex << p_hdr.p_type
			<< " file pos " << hex << p_hdr.p_offset
			<< " at vaddr " << hex << p_hdr.p_vaddr
			<< " at paddr " << hex << p_hdr.p_paddr
			<< " with size "<< dec << p_hdr.p_memsz
			<< " and falgs "<< hex << p_hdr.p_flags
			<< " and align "<< dec << p_hdr.p_align
			<< endl;

		//allocate an virtual memory area at p_vaddr of p_memsz size with
		//p_align alginment and some access flags
		success &= this->allocate_area(
				process,
				p_hdr.p_vaddr,
				p_hdr.p_memsz,
				p_hdr.p_align,
				( p_hdr.p_flags & PF_R ) == PF_R,
				( p_hdr.p_flags & PF_W ) == PF_W,
				( p_hdr.p_flags & PF_X ) == PF_X
				);

		switch ( p_hdr.p_type )
		{
			case PT_NULL:
				//Nothing to do
				break;

			case PT_LOAD:
				//copy content from file at p_offset to p_vaddr with length
				//p_filesz
				success &= this->copy_to_area(
						process,
						file,
						p_hdr.p_offset,
						p_hdr.p_vaddr,
						p_hdr.p_filesz
						);
				break;

			default:
				logger << message::MORE_VERBOSE << __func__
					<< ": Unhandled Segment type(" << p_hdr.p_type
					<< ")" << endl;
				success = false;
				break;
		}
	}

	i_fs::close_node( file );

	if ( !success )
	{
		return false;
	}

	success = this->allocate_stack( process );
	if ( !success )
	{
		return false;
	}

	return this->jump_in(
			process,
			INITIAL_STACK_POINTER,
			elf_header.e_entry,
			args
			);
}

} //namespace elf
} //namespace iposix
