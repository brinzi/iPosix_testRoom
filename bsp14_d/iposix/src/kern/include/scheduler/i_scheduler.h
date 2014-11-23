#ifndef _KERN_INCLUDE_SCHEDULER_I_SCHEDULER_H_
#define _KERN_INCLUDE_SCHEDULER_I_SCHEDULER_H_

#include "config/scheduler.h"
#include "utils/singleton.hpp"
#include <stdint.h>

namespace iposix {
namespace scheduler {

typedef void (*process_entry_function)( uint32_t num, void* data );

template< typename Arch_Scheduler, typename Strategy >
class i_scheduler
	: public Arch_Scheduler,
	public Strategy,
	virtual public ::iposix::utils::Singleton< typename ::iposix::scheduler::i_scheduler< Arch_Scheduler, Strategy > >
{
	friend class ::iposix::utils::Singleton< i_scheduler< Arch_Scheduler, Strategy > >;

	public:
		/**
		 * Creates a new Process like fork but does only copy filehandles and
		 * calls the function
		 * @param[in] fct_ptr the function to run
		 * @param[in] num a parameter to the function
		 * @param[in] data a parameter to the function (note that in case there
		 * is a parameter data given the called function should handle the
		 * delete)
		 * @param[in] kernel_process should the new process created as a kernel
		 * process
		 * @param[in] run_in_kernel should the called function run in kernel
		 * mode
		 */
		bool new_process(
				process_entry_function fct_ptr,
				uint32_t num,
				int  state, //pus de brinzi
				void* data,
				bool kernel_process,
				bool run_in_kernel
				);

		/**
		 * Creates a full copy of the current process
		 * @param[in] kernel should the new process run in kernel mode
		 * @param[in] child_pid reference to a field to set the child pid or 0
		 * for the child
		 */
		bool fork_process( bool kernel, uint32_t& child_pid );

		/**
		 * Exits the current process and switches to the next process
		 * @return true on success (this should never fail)
		 */
		bool exit_process();

		/**
		 * Puts the Init Process (the kernel itself)
		 * @param[in] process0 a pointer to the init process
		 */
		void put_process0( process_type* process0 );

		/**
		 * Performs a context switch if possible
		 */
		static void context_switch();

		void block_process(process_type* process);

		void unblock_process(process_type* process);
	
		void assign_process(process_type* process);
		
		void resign_process(process_type* process);
		
		void initialise(process_type* process);

		void terminate();

	private:
		i_scheduler();

		~i_scheduler();

		uint32_t next_pid;
};

typedef i_scheduler< ARCH_SCHEDULER, SCHEDULER_STRATEGIE > scheduler_type;

} //namespace scheduler
} //namespace iposix

#endif /* !_KERN_INCLUDE_SCHEDULER_I_SCHEDULER_H_ */
