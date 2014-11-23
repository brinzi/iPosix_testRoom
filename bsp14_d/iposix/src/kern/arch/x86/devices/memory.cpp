#include "x86/devices/memory.h"
#include "x86/multiboot/multiboot.h"
#include "logger/logger.h"

namespace iposix {
namespace arch {
namespace x86 {

memory::memory()
	: area_count( 0 )
{
}

memory::~memory()
{
}

void memory::init()
{
	logger << "\n----- memory::init() -----\n";

	const multiboot& mb = ::iposix::utils::Singleton< multiboot >::instance();
	const multiboot::multiboot_info& info = *mb.mb_info;

	if( mb.mem_field_present() )
	{
		logger << "Memory fields present:\n";
		logger << "Lower Memory: "  << dec << info.mem_lower << endl;
		logger << "Upper Memory: " << dec << info.mem_upper << endl;
	}
	
	if( mb.elf_section_field_present() )
	{
		logger << "elf section is present: " << dec << info.syms.elf_sec.num << ", " << info.syms.elf_sec.size << ", " << hex << info.syms.elf_sec.addr << ", " << info.syms.elf_sec.shndx << "\n";
	}
	else
	{
		//TODO -> die();
	}

	logger << "Examining " << dec << info.syms.elf_sec.num << " entries:" << endl;

	Elf32_Shdr * section_ptr = reinterpret_cast< Elf32_Shdr * >( info.syms.elf_sec.addr );

	uint32_t max( 0 );

	logger << message::TOO_VERBOSE;

	for( uint32_t i = 0; i < info.syms.elf_sec.num; ++i )
	{
		if( section_ptr->sh_type )
		{
			uint32_t val = section_ptr->sh_addr + section_ptr->sh_size;

			if( val > max )
			{
				logger << "New highest address: " << hex << val << endl;
				max = val;
			}
		}
		++section_ptr;
	}

	logger << message::VERBOSE;

	if( max & 0x00FFFF )
	{
		max += 0x010000;
	}
	max &= 0xFFFF0000;

	logger << "This Address is the lowest usable: " << hex << max << endl;

	if( mb.memory_map_field_present() )
	{
		logger << "Memory map present:\n";
		logger << "Length: " << dec << info.mmap_length;
		uint32_t size = info.mmap_length / sizeof( multiboot::memory_map );
		logger << " (" << dec << size << " entries)\n" << "Entries:\n";

		const multiboot::memory_map * map = info.mmap_addr;

		for( uint32_t i = 0; i < size; ++i )
		{
			if( map[i].size != 20 )
			{
				if( map[i].size == 24 )
				{
					logger << "ACPI 3.0 extended attributes not supported.\n";
				}
				else
				{
					logger << "ERROR\n";
				}
				continue;
			}

			logger << dec << i << ": Base: " << hex << map[i].base_addr_low << ", ";
			uint32_t msize = map[i].length_low / 1024;
			logger << "size: " << dec << msize << "K, type: ";

			switch( map[i].type )
			{
				case 1:
					logger << "usable";
					break;

				case 2:
					logger << "reserved";
					break;

				case 3:
					logger << "ACPI reclaimable";
					break;

				case 4:
					logger << "ACPI NVS";
					break;

				case 5:
					logger << "bad";
					break;

				default:
					logger << "unknown";
					break;
			}

			if( map[i].base_addr_low == 0 && map[i].length_low <= 640 * 1024 && map[i].type == 1 )
			{
				logger << " <- Base memory, i won't use it";
			}
			else if( map[i].base_addr_low >= 1 * 1024 * 1024 && map[i].type == 1 )
			{
				uint32_t low = map[i].base_addr_low;
				uint32_t high = low + map[i].length_low;

				if( low < max )
				{
					low = max;
				}

				if( low >= high || this->area_count == sizeof( this->areas ) )
				{
					logger << " <- Normal memory, but i can't use it.";
				}
				else
				{
					logger << " <- Normal memory, i will use it";
					this->areas[this->area_count] = memory_area( reinterpret_cast< void * >( low ), reinterpret_cast< void * >( high ) );
					this->area_count += 1;
				}
			}
			else if ( map[i].type == 3 )
			{
				logger << " <- i will not use ACPI memory";
			}
			else
			{
				logger << " <- i will avoid this";
			}

			logger << endl;
		}
	}

	logger.print( "I've found ", dec, this->area_count, " memory areas.", endl );

	logger.print( "Enable PSE and PGE", endl );
	this->init_cr4();

	logger << "----- ~memory::init() -----\n\n";
}

uint32_t
memory::get_memory_areas( memory_area * buffer, uint32_t size ) const
{
	uint32_t max = size < this->area_count ? size : this->area_count;

	for( uint32_t ac = 0; ac < max; ++ac )
	{
		buffer[ac] = this->areas[ac];
	}

	return max;
}

void memory::invalidate_cache() const
{
	asm volatile("wbinvd");
}

void memory::invalidate_translation_lookaside_buffer() const
{
	uintptr_t tmpreg;

	asm volatile(
			"mov %%cr3, %0;\n\t"
			"mov %0, %%cr3;	# invalidate TLB\n\t"
			:	"=b" (tmpreg)
			:	// no inputs
			:
			);
}

void memory::invalidate_translation_lookaside_buffer( const void* addr ) const
{
	asm volatile(
			"invlpg %0"
			: //no outputs
			:"m" ( addr )
			: // no clobber list
			);
}

void memory::set_page_table( void* pagetable_ptr ) const
{
	asm volatile(
			"mov %0, %%cr3	# simply write the pointer to the cr3 reg\n\t"
			:	// no outputs
			: 	"b" ( pagetable_ptr )
			:	// no clobber list
			);
}

void memory::paging( bool enable ) const
{
	uint32_t tmpreg;

	asm volatile(
			"mov %%cr0, %0		# get cr0\n\t"
			:	"=r" ( tmpreg )
			:	// no inputs
			:	// no clobber list
			);

	if ( enable )
	{
		tmpreg |= 0x80000000; // set the paging bit

	}
	else
	{
		tmpreg &= 0x7fffffff; //unset the paging bit
	}

	asm volatile(
			"mov %0, %%cr0		# set cr0\n\t"
			:	// no outputs
			:	"r" ( tmpreg )
			:	// no clobber list
			);
}

void memory::init_cr4() const
{
	uint32_t tmpreg;

	asm volatile(
			"mov %%cr4, %0		# get cr4\n\t"
			:	"=r" ( tmpreg )
			:	// no inputs
			:	// no clobber list
			);

	tmpreg |= 0x00000010; // set the Page Size Enable bit
	tmpreg |= 0x00000080; // set the Page Gloal Enable bit

	asm volatile(
			"mov %0, %%cr4		# set cr4\n\t"
			:	// no outputs
			:	"r" ( tmpreg )
			:	// no clobber list
			);
}

bool memory::paging() const
{
	uint32_t tmpreg;

	asm volatile(
			"mov %%cr0, %0		# get cr0\n\t"
			:	"=r" ( tmpreg )
			:	// no inputs
			:	// no clobber list
			);

	return ( ( tmpreg & 0x80000000 ) == 0x80000000 );
}

scoped_paging_off::scoped_paging_off( bool preserve_old_value )
	: old_value( true )
{
	if ( preserve_old_value )
	{
		this->old_value = ::iposix::utils::Singleton< memory >::instance().paging();
	}

	::iposix::utils::Singleton< memory >::instance().paging( false );
}

scoped_paging_off::~scoped_paging_off()
{
	::iposix::utils::Singleton< memory >::instance().paging( this->old_value );
}

}}} //iposix::arch::x86
