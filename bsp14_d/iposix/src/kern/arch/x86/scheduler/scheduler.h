#ifndef _KERN_ARCH_X86_SCHEDULER_SCHEDULER_H_
#define _KERN_ARCH_X86_SCHEDULER_SCHEDULER_H_

#include "scheduler/i_process.h"

#include <stdint.h>

namespace iposix {
namespace arch {
namespace x86 {

using ::iposix::scheduler::process_type;

/** Function pointer to the function which should be called from the entry
 * function.
 */
typedef void(*start_function)( uint32_t num, void* data );

/** A Struct which describes the birth of a new process */
struct entrance_information_t
{
	/** Magic sequence to denote the entrance information is valid
	 * (0xstart00) */
	static const uint32_t MAGIC = 0x5742700;

	/** Magic sequence above */
	const uint32_t magic_above;
	/** Should the function be called in kernel mode */
	const bool kernel;
	/** The Function to call */
	const start_function fct_ptr;
	/** A parameter to the function */
	const uint32_t num;
	/** A parameter to the function */
	void* data;
	/** Magic sequence under */
	const uint32_t magic_under;

	/**
	 * Constructor
	 * @param[in] kernel Should the function be called in kernel mode
	 * @param[in] fct_ptr The Function to call
	 * @param[in] num A parameter to the function
	 * @param[in] data A parameter to the function
	 */
	entrance_information_t(
			bool kernel,
			start_function fct_ptr,
			uint32_t num,
			void* data
			)
		: magic_above( MAGIC ),
		kernel( kernel ),
		fct_ptr( fct_ptr ),
		num( num ),
		data( data ),
		magic_under( MAGIC )
	{ }

	/**
	 * Calls the start function
	 * @return false only if the function pointer returns or the magic was wrong
	 */
	inline bool call()
	{
		if ( this->magic_above == MAGIC && this->magic_under == MAGIC )
		{
			this->fct_ptr( this->num, this->data );
		}
		return false;
	}
};

/**
 * Does the Architecture depending process preparation and holds the switch_to
 * code
 */
class scheduler
{
	public:
		/**
		 * Switches the current running process
		 * @param[in,out] prev the previous (current) running process it becomes
		 * previous or it is previous because it's not running actually (not
		 * really ) - you'll forget what you have read on the next dot.
		 * @param[in,out] next the next running process
		 * \todo TODO check if there are more registers to save (SSE, etc)
		 * \todo TODO rewrite as pure asm function (see process_schedule_data.h)
		 */
		void switch_to( process_type& prev, process_type& next );

		/**
		 * Does the hardware specific initialisation of a process. This is done
		 * by faking a stack that seems to come from a function where we want to
		 * jump to (or we want to call). So we use the return instruction at the
		 * end of the *switch_to* function to "call" a new function (return is
		 * like a jump but it use the stack top not a hardcoded address).
		 * @param[in,out] process the process which should be prepared
		 * @param[in] run_in_kernel should the called function run in kernel
		 * mode
		 * @param[in] fct_ptr the function to call
		 * @param[in] num a parameter to the function
		 * @param[in] data a parameter to the function
		 */
		bool prepare(
				process_type& process,
				bool run_in_kernel,
				start_function fct_ptr,
				uint32_t num,
				void* data
				);

		/**
		 * Prepares a child for running. Therefore two process returning from
		 * this function the child and the parent.
		 * @param[in] parent the parent of the new child
		 * @param[in] child the new child
		 * @return the pid of the returning process
		 */
		uint32_t prepare(
				process_type& parent,
				process_type& child );

		/**
		 * Sets a process as the current process (does hardware depending stuff
		 * not a context switch)
		 * @param[in] process the process which should be set as current process
		 */
		void set_current( process_type& process );

	protected:
		/** Constructor */
		scheduler();

		/** Destructor */
		virtual ~scheduler();

	private:
		/**
		 * The entry function after the birth of a new process
		 * @param[in] ent_info information about the brith
		 */
		static void entrance_function( entrance_information_t* ent_info );
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_SCHEDULER_SCHEDULER_H_ */
