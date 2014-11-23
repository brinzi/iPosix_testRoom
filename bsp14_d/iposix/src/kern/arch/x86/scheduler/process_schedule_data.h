#ifndef _KERN_ARCH_X86_SCHEDULER_PROCESS_SCHEDULE_DATA_H_
#define _KERN_ARCH_X86_SCHEDULER_PROCESS_SCHEDULE_DATA_H_

#include "utils/noncopyable.h"
#include <stdint.h>

namespace iposix {
namespace arch {
namespace x86 {

/* Predefines */
class scheduler;
struct entrance_information_t;

/** Function pointer to the entry function */
typedef void(*entrance_fct_ptr)( entrance_information_t* ent_info );

/**
 * A struct used to fake the stack for a newly created process.
 */
struct stack_bottom_mask
{
	friend class scheduler;

	private:
		/** The parameter to the entrance function */
		entrance_information_t* entrance_information;

		/** Just to have a buffer */
		uint32_t zero;

		/** To easy find the stack bottom */
		uint32_t BOTTOM4;
		/** To easy find the stack bottom */
		uint32_t BOTTOM3;
		/** To easy find the stack bottom */
		uint32_t BOTTOM2;
		/** To easy find the stack bottom */
		uint32_t BOTTOM1;
};

/**
 * Struct for every object that could be a process and so one must be scheduled.
 * This only holds an esp but is used for stack preparation.
 */
class process_schedule_data
	: virtual public ::iposix::utils::Noncopyable
{
	/* only the scheduler can use the attributes - nice isn't it */
	friend class scheduler;

	public:
	/** Constructor */
	process_schedule_data();

	/** Destructor */
	~process_schedule_data();

	/**
	 * Switches the stack and copies the initial stack content to the processes
	 * kernel stack
	 * @param[in] cur_stack_base the base address of the stack to copy
	 * @param[in] immediate_use should the stack be used immediatly
	 */
	void copy_stack( uintptr_t cur_stack_base, bool immediate_use = true );

	/**
	 * Copies the stack from another process
	 * @param[in] copy_from the process to copy the stack from
	 * @param[in] cur_stack_top upperbound to copy
	 * @param[in] cur_ebp base pointer relying on the stack_top
	 */
	void copy_stack(
			process_schedule_data& copy_from,
			uintptr_t cur_stack_top,
			uintptr_t cur_ebp );

	private:
	/** The kernel stack size */
	static const uint32_t KERNEL_STACK_SIZE = 0x4000; // 16KiB

	/** Describes a stack and makes easy to copy */
	struct kernel_stack_t
	{
		/** The Stack content */
		uint32_t content[KERNEL_STACK_SIZE];
	} kernel_stack;

	public:
	/**
	 * A stack pointer to the base of this processes kernel stack.
	 */
	const uintptr_t kernel_stack_base;

	private:
	union
	{
		/** A mask to fake the stack */
		stack_bottom_mask* fake_stack;

		/** The tasks esp saved during context switch */
		uintptr_t esp;
	};

	union
	{
		/** A pointer to the entrance function used for new processes */
		entrance_fct_ptr fct_ptr;

		/** The tasks eip saved during context switch */
		uintptr_t eip;
	};

	/** The tasks ebp saved during context switch */
	uintptr_t ebp;

	/** The tasks cr3 saved during context switch */
	uintptr_t cr3;
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_SCHEDULER_PROCESS_SCHEDULE_DATA_H_ */
