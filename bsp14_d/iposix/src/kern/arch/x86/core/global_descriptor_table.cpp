#include "x86/core/global_descriptor_table.h"
#include "x86/core/task_state_segment.h"
#include "x86/core/segment_selector.h"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{

global_descriptor_table::global_descriptor_table()
:	reg(),
	table(),
	default_ldt()
{
	//flush all entrys
	for (int i = 0; i < global_descriptor_table::GDT_SIZE; ++i)
	{
		set_null_pointer_segment( i );
	}
}

global_descriptor_table::~global_descriptor_table()
{ }

void global_descriptor_table::init_gdt()
{
	//set up segments
	set_null_pointer_segment();
	set_global_kernel_code_segment();
	set_global_kernel_data_segment();
	set_global_user_code_segment();
	set_global_user_data_segment();
}

void global_descriptor_table::activate_gdt()
{
	this->reg = global_descriptor_table_register( GDT_SIZE, &table[0] );

	segment_selector data_segment_selector = 
		segment_selector( GLOBAL_KERNEL_DATA_SEGMENT_NR );

	segment_selector code_segment_selector =
		segment_selector( GLOBAL_KERNEL_CODE_SEGMENT_NR );

	asm volatile(
			"lgdt %0\n\t"
			"mov %1, %%ds				# save this offset to every segment register\n\t"
			"mov %1, %%es\n\t"
			"mov %1, %%fs\n\t"
			"mov %1, %%gs\n\t"
			"mov %1, %%ss\n\t"
			"push %2					# push the segment selector\n\t"
			"mov $label_%=, %%ebx\n\t"
			"push %%ebx 				# push the jump destination\n\t"
			"lret 						# jump okay we pretend to be interrupted\n"
			"label_%=: nop				# jump destination"
			: /* no outputs */
			:	"m" ( this->reg ), 
				"r" ( data_segment_selector.bits ), 
				"m" ( code_segment_selector.bits )
			:	"eax", 
				"ebx" 
				  );
}

void global_descriptor_table::activate_ldt(const uint16_t n)
{
	segment_selector ldtr = segment_selector( n );

	asm volatile("lldt %0" : : "m" (ldtr));
}

void global_descriptor_table::set_null_pointer_segment(uint16_t n)
{
	this->table[n] = 
		memory_descriptor( 
				memory_descriptor::null_pointer_segment_descriptor);
}

void global_descriptor_table::set_global_kernel_code_segment()
{
	this->table[GLOBAL_KERNEL_CODE_SEGMENT_NR] = memory_descriptor( memory_descriptor::kernel_code_segment_descriptor, true );
}

void global_descriptor_table::set_global_kernel_data_segment()
{
	this->table[GLOBAL_KERNEL_DATA_SEGMENT_NR] = memory_descriptor( memory_descriptor::kernel_data_segment_descriptor, true );
}

void global_descriptor_table::set_global_user_code_segment()
{
	this->table[GLOBAL_USER_CODE_SEGMENT_NR] = memory_descriptor( memory_descriptor::user_code_segment_descriptor, true );
}

void global_descriptor_table::set_global_user_data_segment()
{
	this->table[GLOBAL_USER_DATA_SEGMENT_NR] = memory_descriptor( memory_descriptor::user_data_segment_descriptor, true );
}

void global_descriptor_table::set_task_state_segment(
		uint16_t n, 
		const task_state_segment* tss)
{
	if ((n >= global_descriptor_table::FIRST_FREE_SEGMENT) && (n < global_descriptor_table::GDT_SIZE))
	{
		table[n] = memory_descriptor(
				memory_descriptor::task_state_segment_descriptor,
				true,
				reinterpret_cast<uint32_t>(tss),
				reinterpret_cast<uint32_t>(tss) + sizeof(task_state_segment),
				true);
	}
}

void global_descriptor_table::set_local_descriptor_table_segment(const uint16_t n)
{

	if ((n >= global_descriptor_table::FIRST_FREE_SEGMENT) && (n < global_descriptor_table::GDT_SIZE))
	{
		table[n] = memory_descriptor(
				memory_descriptor::local_descriptor_table_descriptor,
				true,
				reinterpret_cast<uint32_t>(&default_ldt[0]),
				DEFAULT_LDT_SIZE);
		
	}
}

void global_descriptor_table::dump()
{
	for ( uint32_t i = 0; i < GDT_SIZE; i++ )
	{
		this->table[i].dump();
	}
}

} //namespace x86
} //namespace arch
} //namespace iposix
