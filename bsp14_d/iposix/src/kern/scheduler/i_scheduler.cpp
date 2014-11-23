#include "scheduler/i_scheduler.h"
#include "scheduler/i_process.h"
#include "logger/logger.h"

namespace iposix {
namespace scheduler {

template<>
scheduler_type::i_scheduler()
	: next_pid( 1 )
{ }

template<>
scheduler_type::~i_scheduler()
{ }

template<>
bool scheduler_type::new_process(
		process_entry_function fct_ptr,
		uint32_t num,
		int state, //pus de Brinzi
		void* data,
		bool kernel_process,
		bool run_in_kernel
		)
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Creating new Process with pid " << this->next_pid << endl;

	process_type* cur = this->get_current_process();
	if ( !cur )
	{
		return false;
	}

	process_type* child = new process_type( kernel_process );
	if ( child )
	{
		child->pid = this->next_pid++;

		//copy addressspace
		bool success = child->copy_address_space( *cur );
		if ( !success )
		{
			delete child;
			return false;
		}

		//copy filehandles
		child->copy_filehandles( *cur );

		//prepare for running
		this->prepare( *child, run_in_kernel, fct_ptr, num, data );

		//add to scheduler
		this->push_new( child );
	}

	return true;
}

template<>
bool scheduler_type::fork_process( bool kernel, uint32_t& child_pid )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Forking new Process with pid " << this->next_pid << endl;

	process_type* cur = this->get_current_process();
	if ( !cur )
	{
		return false;
	}

	process_type* child = new process_type( kernel );
	if ( child )
	{
		child->pid = this->next_pid++;
		child_pid = child->pid;
		
		uint32_t parent_pid = cur->pid;
		uint32_t ret_pid = 0;

		//copy addressspace
		bool success = child->copy_address_space( *cur );
		if ( !success )
		{
			delete child;
			return false;
		}

		//copy filehandles
		child->copy_filehandles( *cur );

		ret_pid = this->prepare( *cur, *child );

		if ( ret_pid != parent_pid )
		{
			//the child
			child_pid = 0;
		}
		else
		{
			//the parent
			this->push_new( child );
		}

		return true;
	}
	else
	{
		return false;
	}
}

template<>
bool scheduler_type::exit_process()
{
	return false;
}

template<>
void scheduler_type::put_process0( process_type* process0 )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Take Process 0" << endl;

	process0->pid = 0;

	this->push_new( process0 );

	this->set_current( *process0 );
}

template<>
void scheduler_type::context_switch()
{
	scheduler_type& instance = ::iposix::utils::Singleton< scheduler_type >::instance();

	process_type* current = instance.get_current_process();
	process_type* next = instance.get_next_process();

	if ( current && next && current != next )
	{
		//do the context switch
		instance.switch_to( *current, *next );
	}
}

/*Move process to blocked*/	
template<>
void scheduler_type::block_process(process_type* process)
{	
	//Switch context so that data will be preserved.
	this->context_switch();

	//Set's status to 0=blocked.
	process->state=0;

	//Adds process to the blocked queue.
	process->move_to_blocked(process);

	//Removes process from ready queue.
	process->remove(process);

}
/*Move process to ready queue.*/
template<>
void scheduler_type::unblock_process(process_type* process)
{
	//Switch context.
	this->context_switch();

	//Set the status of the process to 1=ready.
	process->state=1;

	//Move process to ready list.
	process->move_to_ready(process);

	//Remove from blocked
	process->remove_from_blocked(process);
}
/*Take the process from ready to running.*/
template<>
void scheduler_type::assign_process(process_type* process)
{	
	this->push_new(process);
	
	this->set_current(*process);
}
/*Take the process from running to ready*/
template<>
void scheduler_type::resign_process(process_type* process)
{	
	this->context_switch();	

	this->push_new(process);
}
/*Move from inactive to ready*/
template<>
void scheduler_type::initialise(process_type* process)
{
	this->push_new(process);
	
	this->remove_from_inactive(process);
}
/*Terminates the process calling exit_process*/
template<>
void scheduler_type::terminate()
{
	this->exit_process();
}
} //namespace scheduler
} //namespace iposix
