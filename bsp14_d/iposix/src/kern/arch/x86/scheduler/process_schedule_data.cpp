#include "x86/scheduler/process_schedule_data.h"
#include "logger/logger.h"

namespace iposix {
namespace arch {
namespace x86 {

process_schedule_data::process_schedule_data()
	: kernel_stack_base(
			reinterpret_cast<uintptr_t> (
				&kernel_stack.content[KERNEL_STACK_SIZE - 1]
				)
			)
{ }

process_schedule_data::~process_schedule_data()
{ }

inline void copy_and_adapt_stack(
		uintptr_t old_stack_base,
		uintptr_t old_stack_top,
		uintptr_t old_ebp,
		uintptr_t new_stack_base,
		uintptr_t& new_stack_top,
		uintptr_t& new_ebp
		)
{
	intptr_t movement = new_stack_base - old_stack_base;
	uintptr_t old_stack_size = old_stack_base - old_stack_top;

	logger << message::MORE_VERBOSE << __func__
		<< ": Copying stack from " << hex << old_stack_base
		<< " to " << hex << new_stack_base
		<< " with length " << dec << old_stack_size
		<< " and stack movement " << hex << movement << dec << endl;

	uint32_t* it_in = reinterpret_cast<uint32_t*>( new_stack_base );
	uint32_t* it_out = reinterpret_cast<uint32_t*>( old_stack_base );

	//copy the stack
	for ( ; old_stack_size > 0; old_stack_size -= sizeof( uint32_t ) )
	{
		//assume that all values on the stack inside the range of the old stack
		//are base pointers so we move the values to fit the new stack
		if (
				( old_stack_top < ( *it_out ) ) &&
				( ( *it_out ) < old_stack_base )
		   )
		{
			logger << "\tmoving possible ebp "
				<< hex << *it_out << dec << endl;
			*(it_in--) = *(it_out--) + movement;
		}
		else
		{
			*(it_in--) = *(it_out--);
		}
	}

	new_stack_top = old_stack_top + movement;
	new_ebp = old_ebp + movement;
}

void process_schedule_data::copy_stack(
		uintptr_t cur_stack_base,
		bool immediate_use
		)
{
	uintptr_t cur_stack_top;
	uintptr_t cur_ebp;

	asm volatile(
			"mov %%esp, %0	#get stack_pointer\n\t"
			"mov %%ebp, %1	#get base_pointer\n\t"
			: "=r" (cur_stack_top),
			"=r" (cur_ebp)
			);

	copy_and_adapt_stack(
			cur_stack_base,
			cur_stack_top,
			cur_ebp,
			this->kernel_stack_base,
			this->esp,
			this->ebp);

	if ( immediate_use )
	{
		asm volatile(
				"mov %0, %%esp	#get stack_pointer\n\t"
				"mov %1, %%ebp	#get base_pointer\n\t"
				:
				:	"r" (this->esp),
				"r" (this->ebp)
				);
	}
}

void process_schedule_data::copy_stack(
		process_schedule_data& copy_from,
		uintptr_t cur_stack_top,
		uintptr_t cur_ebp )
{
	copy_and_adapt_stack(
			copy_from.kernel_stack_base,
			cur_stack_top,
			cur_ebp,
			this->kernel_stack_base,
			this->esp,
			this->ebp);
}

} //namespace x86
} //namespace arch
} //namespace iposix
