#include "tests/tests.h"
#include "elf/i_elf_loader.h"
#include "scheduler/i_scheduler.h"
#include "x86/core/interrupts.h"
#include "logger/logger.h"

namespace iposix {
namespace tests {

using ::iposix::elf::elf_loader_type;
using ::iposix::scheduler::scheduler_type;

void load_init( uint32_t /*num*/, void* /*data*/ )
{
	elf_loader_type& el = ::iposix::utils::Singleton< elf_loader_type >::instance();

	el.load_elf( "/bin/abtest" , 0 );

	while ( true );
}

bool elf_loader_test()
{
	::iposix::utils::Singleton< scheduler_type >::instance().new_process(
	&load_init,0, 0, 0, false, true );

	bool success( false );

	::iposix::arch::x86::interrupt::interrupts_on();
	while ( true );

	return success;
}

} //namespace tests
} //namespace iposix
