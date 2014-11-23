#include "scheduler/i_process.h"
#include "scheduler/i_scheduler.h"

namespace iposix { 
namespace scheduler {

template<>
i_process< PROCESS >::i_process( bool kernel )
	: PROCESS( kernel ),
	  pid(),
	  state(0),	//pus de mine 
	  kernel( kernel ),
	  heap_top( 0 ),
	  heap_max( 0 )
{ }

process_type*
current_process()
{
	scheduler_type& shd = ::iposix::utils::Singleton< scheduler_type >::instance();

	return shd.get_current_process();
}

}} // iposix::scheduler
