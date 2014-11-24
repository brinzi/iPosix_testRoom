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

				struct inactive_process_list{
					inactive_process_list* inactive_next;	

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
					inactive_list_head(0),
					blocked_list_head(0),
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

					if ( pl)
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



				//Added by Nica

				//adds a process to the inactive list	
				void move_to_inactive(process_type* process)
				{

					inactive_process_list* aux = new inactive_process_list();
					//remeber process
					aux->process=process;

					//add process to list
					aux->inactive_next=this->inactive_list_head;
					this->inactive_list_head=aux;

				}


				//remove  porcess from blocked list and return process
				process_type* remove_from_blocked(process_type* process ){
					blocked_process_list* aux =this->blocked_list_head;
					blocked_process_list* prev = 0;
					while(aux &&(aux->process == process))
					{
						prev=aux;
						aux=aux->blocked_next;	
					}
					if(cur&&(cur->process=process))
					{
						if(prev)
						{
							prev->blocked_next=aux->blocked_next;	
						}else if(aux==this->blocked_list_head){
							this->blocked_list_head=aux->blocked_next;
						}else{
							//exception
						}
					process_type* p= aux->process;
					aux->process=0;
					delete aux;
					return p;	
					}
				return 0;										
					
				}

					
				


				//adds a process to blocked	
				void  move_to_blocked(process_type* process)
				{	

					blocked_process_list* aux=new blocked_process_list();
					aux->process=process;
					aux->blocked_next=this->blocked_list_head;
					this->blocked_list_head=aux;


				}



				private:
				/** A Pointer to the head of the list */
				process_list* list_head;

				/**Added by Brinzi-pointers to the head of the rady list and blocked one. */
				inactive_process_list* inactive_list_head;
				blocked_process_list* blocked_list_head;


				/** A Pointer to the current process in the list */
				process_list* cur;

			};

		} //namespace policies
	} //namespace scheduler
} //namespace iposix

#endif /* !_KERN_INCLUDE_SCHEDULER_POLICIES_ROUND_ROBIN_HPP_ */

