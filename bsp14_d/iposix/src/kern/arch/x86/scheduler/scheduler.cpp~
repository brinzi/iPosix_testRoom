#include "x86/scheduler/scheduler.h"
#include "devices/i_core_holder.h"
#include "scheduler/i_process.h"

namespace iposix {
namespace arch {
namespace x86 {

using ::iposix::arch::core_holder_type;
using ::iposix::arch::core_type;

/**
 * This Funktion returns the callees return eip
 * @return the callees eip
 */
extern "C" uintptr_t get_eip();

static const uintptr_t MAGIC_VALUE = 0xCAFFEE;

scheduler::scheduler()
{ }

scheduler::~scheduler()
{ }

void scheduler::switch_to( process_type& prev, process_type& next )
{
	uintptr_t esp;
	uintptr_t ebp;
	uintptr_t eip;
	uintptr_t cr3;

	asm volatile(
			"mov %%esp, %0	#get stack_pointer\n\t"
			"mov %%ebp, %1	#get base_pointer\n\t"
			"mov %%cr3, %2	#get cr3\n\t"
			: "=r" (esp),
			"=r" (ebp),
			"=r" (cr3)
			: // no inputs
			: // no clobber list
			);

	// we call this function to get the eip of the next instruction
	// so we are able to jump there afterwards and set a new value as return
	// (the MAGIC_VALUE )
	eip = get_eip();

	if ( eip == MAGIC_VALUE )
	{
		//context switch completet
		return;
	}

	//save old
	prev.esp = esp;
	prev.ebp = ebp;
	prev.eip = eip;
	prev.cr3 = cr3;

	//switch to new interrupt kernel stack
	this->set_current( next );

	//restore new
	esp = next.esp;
	ebp = next.ebp;
	eip = next.eip;
	cr3 = next.cr3;
	
	//set base_pointer, stack_pointer, page_table_pointer
	//and jump to value of eip (Note: we put a magic value in eax so get_eip
	//will return this value
	asm volatile(
			"mov %0, %%ebp			#set ebp\n\t"
			"mov %1, %%esp			#set esp\n\t"
			"mov %2, %%cr3			#set cr3\n\t"
			"mov %3, %%ecx			#get eip in ecx\n\t"
			"mov $0xCAFFEE, %%eax	#put magic value in eax\n\t"
			"jmp *%%ecx"
			: // no outputs
			:
			"r" (ebp),
			"r" (esp),
			"r" (cr3),
			"r" (eip)
			: // no clobber list
			);
}

bool scheduler::prepare(
		process_type& process,
		bool run_in_kernel,
		start_function fct_ptr,
		uint32_t num,
		void* data
		)
{
	//set cr3
	process.cr3 = process.page_table_ptr();

	//set eip
	process.fct_ptr = entrance_function;

	//set ebp to dummy value
	process.ebp = 0;

	//put values on the stack
	//set esp
	process.esp = process.kernel_stack_base - sizeof(stack_bottom_mask);

	process.fake_stack->entrance_information =
		new entrance_information_t( run_in_kernel, fct_ptr, num, data );

	logger << message::MORE_VERBOSE << __func__
		<< ": creating entrance information at "
		<< hex
		<< reinterpret_cast<uintptr_t>(
				process.fake_stack->entrance_information
				)
		<< dec << endl;

	if ( process.fake_stack->entrance_information == 0 )
	{
		return false;
	}

	process.fake_stack->zero = 0;

	process.fake_stack->BOTTOM4 = 0xDEADDEAD;
	process.fake_stack->BOTTOM3 = 0xDEADDEAD;
	process.fake_stack->BOTTOM2 = 0xDEADDEAD;
	process.fake_stack->BOTTOM1 = 0xDEADDEAD;

	process.esp -= sizeof( uintptr_t );

	return true;
}

uint32_t scheduler::prepare(
		process_type& parent,
		process_type& child )
{
	uintptr_t esp;
	uintptr_t ebp;
	uintptr_t eip;
	uint32_t child_pid = child.pid;
	uint32_t parent_pid = parent.pid;

	asm volatile(
			"mov %%esp, %0	#get stack_pointer\n\t"
			"mov %%ebp, %1	#get base_pointer\n\t"
			: "=r" (esp),
			"=r" (ebp)
			: // no inputs
			: // no clobber list
			);

	// we call this function to get the eip of the next instruction
	// so we are able to jump there afterwards and set a new value as return
	// (the MAGIC_VALUE )
	eip = get_eip();

	if ( eip == MAGIC_VALUE )
	{
		return child_pid;
	}

	child.copy_stack( parent, esp, ebp );

	//prepare child - esp and ebp are set by copy_stack
	child.cr3 = child.page_table_ptr();
	child.eip = eip;

	return parent_pid;
}

void scheduler::entrance_function( entrance_information_t* ent_info )
{
	if ( ent_info != 0 )
	{
		entrance_information_t copy = *ent_info;

		ent_info->data = 0;

		logger << message::MORE_VERBOSE << __func__
			<< ": deleting entrance information at "
			<< hex << reinterpret_cast<uintptr_t>( ent_info )
			<< dec << endl;

		delete ent_info;

		if ( !copy.kernel )
		{
			//NOTE: you are unable to call kernel functions afterwarts from
			//now one you musst use syscalls :D and these are not available
			//from inside
			//okay you can use kernel functions but you musst make sure that
			//the functions you are calling are not using any privileged
			//instructions.
			//so good luck
			core_type::switch_to_user_mode();
		}

		copy.call();
	}

	//TODO do some shutdown stuff
	while( true );
}

void scheduler::set_current( process_type& process )
{
	//switch to new interrupt kernel stack
	::iposix::utils::Singleton< core_holder_type >::instance().current_core().switch_interrupt_task( process.kernel_stack_base, process.cr3 );
}

} //namespace x86
} //namespace arch
} //namespace iposix
