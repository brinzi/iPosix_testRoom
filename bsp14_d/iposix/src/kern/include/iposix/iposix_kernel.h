#ifndef _KERN_INCLUDE_IPOSIX_IPOSIX_KERNEL_H_
#define _KERN_INCLUDE_IPOSIX_IPOSIX_KERNEL_H_

#include "config/kernel.h"
#include "utils/singleton.hpp"
#include "scheduler/i_process.h"

namespace iposix
{

/**
 * This Class describes the Operation System.
 * This is Singleton
 */
template< typename Architecture, typename Memory, typename Scheduler >
class iposix_kernel 
	: public Architecture,
	public ::iposix::utils::Singleton< typename ::iposix::iposix_kernel< Architecture, Memory, Scheduler> >,
	public ::iposix::scheduler::process_type
{
	typedef iposix_kernel< Architecture, Memory, Scheduler > kernel_type;

	friend class ::iposix::utils::Singleton< kernel_type >;

	public:
		typedef Memory memory_type;

		typedef Scheduler scheduler_type;

		/**
		 * Constructor
		 */
		iposix_kernel()
			: ::iposix::scheduler::process_type( true )
		{
			//process
			this->pid = 0;
			//current_process( *this );
			//~process
		};

		/**
		 * Destructor
		 */
		~iposix_kernel()
		{ };

		/**
		 * Runs the booting stuff. Should be called before run().
		 */
		void boot();

		/**
		 * Runs the Operation System. Should call the initial Process.
		 */
		void run();

		/**
		 * Should handle all the things needed for a clean shutdown.
		 * Should be called after boot() and run().
		 */
		void shutdown();

		/**
		 * Stops the Operation System (Emeergency exit).
		 */
		void stop();
	private:

		/**
		 * Is the System currently running?
		 */
		bool running;
};

template class iposix_kernel<ARCHITECTURE, MEMORY, SCHEDULER>;

typedef iposix_kernel<ARCHITECTURE, MEMORY, SCHEDULER> kernel;

} //namespace iposix

#endif /* !_KERN_INCLUDE_IPOSIX_IPOSIX_KERNEL_H_ */
