#include "tests/tests.h"
#include "scheduler/i_scheduler.h"
#include "x86/core/interrupts.h"
#include "logger/logger.h"

namespace iposix {
namespace tests {

static volatile bool wait;

using ::iposix::scheduler::scheduler_type;

void stop_waiting( uint32_t /*num*/, void* /*data*/ )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": angekommen" << endl;

	wait = false;

	logger << message::MORE_VERBOSE << __func__
		<< ": Switching interrupts on and wait" << endl;
	//::iposix::arch::x86::interrupt::interrupts_on();

	while( true );
}

bool process_test()
{
	logger << message::MORE_VERBOSE << __func__
		<< ": angekommen" << endl;

	wait = true;

	scheduler_type& scheduler = ::iposix::utils::Singleton< scheduler_type >::instance();

	scheduler.new_process( &stop_waiting,0, 0, 0, false, true );//pus de Brinzi

	logger << message::MORE_VERBOSE << __func__
		<< ": Switching interrupts on and wait" << endl;
	::iposix::arch::x86::interrupt::interrupts_on();

	while ( wait );

	logger << message::MORE_VERBOSE << __func__
		<< ": Switching interrupts off" << endl;
	::iposix::arch::x86::interrupt::interrupts_off();
	
	return true;
}

} //namespace tests
} //namespace iposix
