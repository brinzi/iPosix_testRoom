#ifndef _KERN_INCLUDE_SCHEDULER_POLICIES_ROUND_ROBIN_HPP_
#define _KERN_INCLUDE_SCHEDULER_POLICIES_ROUND_ROBIN_HPP_

#include "scheduler/i_process.h"

#include <stdint.h>

namespace iposix {
namespace scheduler {

/**
 * Contains a round robin strategie
 */
namespace policies {

/**
 * Implements a round robin strategie with a single headed list
 */
class round_robin
{
	/**
	 * A Single headed list Structure
	 */
	struct process_list
	{
		/** The next list structure */
		process_list* next;	
		/** The underlying process */
		process_type* process;
	};

	//pus de Brinzi

	struct ready_process_list
	{
		ready_process_list* ready_next;	
		process_type* process;
	};	

	struct blocked_process_list
	{
		blocked_process_list* blocked_next;
		process_type* process; 
	};

	

	protected:
		/** Constructor */
		round_robin()
			: list_head( 0 ),
			cur( 0 )
		{ }

		/** Destructor */
		virtual ~round_robin()
		{ }

	public:
		/**
		 * Return the current running process
		 * @return the current running process
		 */
			process_type* get_current_process() const
		{
			if ( this->cur )
			{
				return this->cur->process;
			}
			else
			{
				//Hm there is no current
				return 0;
			}
		}

		/**
		 * Returns the next ready process
		 * @return the next ready process
		 */
		process_type* get_next_process()
		{
			if ( this->cur )
			{
				if ( this->cur->next )
				{
					//got a next so take him
					this->cur = this->cur->next;
				}
				else
				{
					//no next start from beginning
					this->cur = this->list_head;
				}

				if ( this->cur )
				{
					return this->cur->process;
				}
			}

			//Hm there is no next
			return 0;
		}

		/**
		 * Add a new process to the queue
		 * @param[in] process the process to add
		 */
		void push_new( process_type* process )
		{
			process_list* pl = new process_list();

			if ( pl )
			{
				//remember process
				pl->process = process;

				//set old head as next
				pl->next = this->list_head;

				//conditionaly set as current if there is no
				if ( !this->cur )
				{
					if ( this->list_head )
					{
						this->cur = this->list_head;
					}
					else
					{
						this->cur = pl;
					}
				}

				//push as new beginning
				this->list_head = pl;
			}
			else
			{
				//TODO handle error case - throw exception?
			}
		}


		/**
		 * Removes a process from the queue
		 * @param[in] process the process to remove
		 * @return the process which has to be delete (callee should delete the
		 * process



		 */
		process_type* remove( process_type* process )
		{
			process_list* cur = this->list_head;
			process_list* prev = 0;

			//iterator the list
			while ( cur && ( cur->process != process ) )
			{
				prev = cur;
				cur = cur->next;
			}

			if ( cur && ( cur->process == process ) )
			{
				//found
				if ( prev )
				{
					prev->next = cur->next;
				}
				else if ( cur == this->list_head )
				{
					this->list_head = cur->next;
				}
				else
				{
					//TODO internal error
				}

				//remeber the process
				process_type* p = cur->process;

				//delete the list structure
				cur->process = 0;
				delete cur;
				
				//return the process
				return p;
			}

			//Hm nothing found
			return 0;
		}

			
		static inline uint64_t ticks_between_reschedulings()
		{
			return 1000;
		}
		
		void move_to_ready(process_type* process)
		{
			ready_process_list* cur_ready=this->ready_list_head;
			ready_process_list* aux = new ready_process_list();
			if(aux)
			{
				aux->process=process;
				aux->ready_next=this->ready_list_head;
			
				if(!this->cur_ready)
				{
					if(this->ready_list_head)
					{
						this->cur_ready=this->ready_list_head;
					}
					else
					{
						this->cur_ready=aux;
					}
			
			this->ready_list_head=aux;
			}
		}
}		

		void  move_to_blocked(process_type* process)
		{	
			blocked_process_list* cur_blocked=this->cur_blocked;

			blocked_process_list* aux=new blocked_process_list();

			if(!this->cur_blocked)
			{
				aux->process=process;
				aux->blocked_next= this->blocked_list_head;
				if(this->blocked_list_head)
				{
					this->cur_blocked=this->blocked_list_head;
				}
				else
				{	
					this->cur_blocked=aux;
				}
			this->blocked_list_head=aux;
			}	
		}
			
	private:
		/** A Pointer to the head of the list */
		process_list* list_head;
		
		/**Added by Brinzi-pointers to the head of the rady list and blocked one. */
		ready_process_list* ready_list_head;
		blocked_process_list* blocked_list_head;
  

		/** A Pointer to the current process in the list */
		process_list* cur;
		ready_process_list* cur_ready;
		blocked_process_list* cur_blocked;

};

} //namespace policies
} //namespace scheduler
} //namespace iposix

#endif /* !_KERN_INCLUDE_SCHEDULER_POLICIES_ROUND_ROBIN_HPP_ */
