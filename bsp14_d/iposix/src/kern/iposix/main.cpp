#include "iposix/iposix_kernel.h"
#include "tests/tests.h"
#include "logger/logger.h"
#include "tests/tests.h"

#include "die.h"

#include <exception>
#include <stdint.h>

namespace iposix
{

/**
 * This initializes the memory.
 */
extern "C" void memory_init()
{
	try
	{
		MEMORY& mem = ::iposix::utils::Singleton< MEMORY >::instance();

		try
		{
			logger << message::VERBOSE << "Setting up Memory... " << endl;
			mem.init();
		}
		catch ( ::std::exception& e )
		{
			logger << message::CRITICAL << __func__
				<< "Exception: " << e.what() << endl;
		}
	}
	catch ( ... )
	{
		die();
	}
}

/**
 * This is the main entry after every thing is properply set up (libraries,
 * etc). This calls the Kernel specific boot procedure, run procedure and
 * shutdown procedure.
 */
extern "C" void kernel_main( uintptr_t initial_stack_pointer )
{
	try
	{
		::iposix::kernel& kern = ::iposix::utils::Singleton< ::iposix::kernel >::instance();

		try
		{
			kern.copy_stack( initial_stack_pointer );
			kern.boot();
			::iposix::tests::all_tests();
			kern.run();
			kern.shutdown();
		}
		catch ( ::std::exception& e )
		{
			// try to print the exception
			logger << message::HIGHEST << __func__
				<< "Exception: " << e.what() << endl;

			// try to shutdown
			kern.shutdown();
		}

		logger << message::HIGHEST << "System will halt now..." << endl;
	}
	catch ( ... )
	{
		die();
	}

}

//int main(int argv, char ** args)
//{
//}

} //namespace iposix
