#include "x86/core/cpu_core.h"
#include "x86/core/segment_selector.h"
#include "core/service_routine.h"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{

cpu_core::cpu_core()
:	gdt(),
	idt(),
	interrupt_task_tss()
{ }

cpu_core::~cpu_core()
{ }

void cpu_core::init()
{
	logger << message::VERBOSE
		<< "Setting up Global Descriptor Table... " << endl;
	this->gdt.init_gdt();

	logger << message::VERBOSE
		<< "Setting up Task State Segment..." << endl;
	this->init_task_state_segment();

	logger << message::VERBOSE
		<< "Setting up Interrupt Descriptor Table... " << endl;
	this->idt.init_idt();

	logger << message::MORE_VERBOSE
		<< "Activating the Global Descriptor Table..." << endl;
	this->gdt.activate_gdt();

	logger << message::MORE_VERBOSE
		<< "Activating the Interrupt Descriptor Table..." << endl;
	this->idt.activate_idt();

	logger << message::MORE_VERBOSE
		<< "Activating the Task State Segment..." << endl;
	this->activate_task_state_segment();

}

void cpu_core::init_task_state_segment()
{
	//create task state segment descriptor entry in the GDT
	this->gdt.set_task_state_segment(
			global_descriptor_table::INTERRUPT_TASK_TSS_NR,
			&this->interrupt_task_tss );

	//create local descriptor table descriptor in the GDT
	this->gdt.set_local_descriptor_table_segment(
			global_descriptor_table::INTERRUPT_TASK_LDT_NR );
}

void cpu_core::activate_task_state_segment()
{
	// musst be done for reseting busy bit
	this->gdt.set_task_state_segment(
			global_descriptor_table::INTERRUPT_TASK_TSS_NR,
			&this->interrupt_task_tss );

	this->gdt.dump();

	this->interrupt_task_tss.dump();

	//set task register
	segment_selector tssr =
		segment_selector(
				global_descriptor_table::INTERRUPT_TASK_TSS_NR,
				privilege::user_privilege_level
				);

	logger << message::MORE_VERBOSE << __func__
		<< ": Setting up Task Segment Register for TSS at "
		<< hex << reinterpret_cast<uintptr_t>( &this->interrupt_task_tss )
		<< " with value "
		<< tssr.bits << hex
		<< endl;

	asm volatile("ltr %0" : : "r" (tssr.bits));

	//set local descriptor table register
	segment_selector ldtr =
		segment_selector(
				global_descriptor_table::INTERRUPT_TASK_LDT_NR,
				privilege::system_privilege_level
				);

	this->gdt.activate_ldt(
			global_descriptor_table::INTERRUPT_TASK_LDT_NR
			);
}

void cpu_core::switch_interrupt_task(
		uintptr_t stack_pointer_addr,
		uintptr_t page_table_ptr )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Setting interrupt kernel stack "
		<< hex << stack_pointer_addr
		<< " and interrupt page table "
		<< page_table_ptr << dec
		<< endl;

	this->interrupt_task_tss.ESP0 =
		reinterpret_cast<uint32_t*>(stack_pointer_addr);

	this->interrupt_task_tss.CR3 = page_table_ptr;
}

void cpu_core::register_irq_handler(
		const uint16_t irq, const
		service_routine h)
{
	interrupt_descriptor_table::register_irq_handler( irq, h );
}

void cpu_core::remove_irq_handler(
		const uint16_t irq)
{
	interrupt_descriptor_table::remove_irq_handler( irq );
}

void cpu_core::enable_irq_handler(
		const uint16_t irq)
{
	interrupt_descriptor_table::enable_irq_handler( irq );
}

void cpu_core::disable_irq_handler(
		const uint16_t irq)
{
	interrupt_descriptor_table::disable_irq_handler( irq );
}

void cpu_core::set_irq_usability(
				int entry_number, bool present )
{
	this->idt.set_irq_usability( entry_number, present );
}

void cpu_core::switch_to_user_mode()
{
	segment_selector data_segment_selector = 
		segment_selector(
				global_descriptor_table::GLOBAL_USER_DATA_SEGMENT_NR,
				privilege::user_privilege_level);

	segment_selector code_segment_selector =
		segment_selector(
				global_descriptor_table::GLOBAL_USER_CODE_SEGMENT_NR,
				privilege::user_privilege_level);

	asm volatile(
			"cli					# switch of interrupts\n\t"
			"mov $0, %%eax			\n\t"
			"mov $0, %%ecx			\n\t"
			"mov $0, %%edx			\n\t"
			"mov %0, %%edx			# get the data_segment_selector\n\t"
			"mov %1, %%ecx			# get the code_segment_selector\n\t"
			"mov %%dx, %%ds			# save this to every segment register\n\t"
			"mov %%dx, %%es			\n\t"
			"mov %%dx, %%fs			\n\t"
			"mov %%dx, %%gs			# but not to stack segment selector\n\t"
			"mov %%esp, %%eax		# get the stack pointer\n\t"
			"pushl %%edx			# now we push the stack segment selector\n\t"
			"pushl %%eax			# we push the stack pointer\n\t"
			"pushf					# we push the eflags register\n\t"
			"popl %%eax				# we pop the eflags again\n\t"
			"or $0x200, %%eax		# activate interrupt enable flag\n\t"
			"pushl %%eax			# after modfiy push it again\n\t"
			"pushl %%ecx			# now we push the code segment selector\n\t"
			"mov $label_%=, %%eax	# get the jump destination\n\t"
			"pushl %%eax			# push the jump destination\n\t"
			"iret					# jump okay we pretend to be an interrupt\n"
			"label_%=: nop			# we should be in user mode now\n\t"
			: /* no outputs */
			:	"m" ( data_segment_selector.bits ), 
				"m" ( code_segment_selector.bits )
			:	"eax", 
				"ecx",
				"edx"
			);
}

uint64_t cpu_core::get_current_cpu_time_stamp()
{
	union int_64
	{
		uint64_t i64;
		uint32_t i32[2];
	} i;

	asm volatile("rdtsc" : "=a" ( i.i32[0] ), "=d" ( i.i32[1] ) );

	return i.i64;
}

} //namespace x86
} //namespace arch
} //namespace iposix
