#ifndef _KERN_INCLUDE_SCHEDULER_POLICIES_PRIORITY_ROUND_ROBIN_HPP_
#define _KERN_INCLUDE_SCHEDULER_POLICIES_PRIORITY_ROUND_ROBIN_HPP_

#include "scheduler/i_process.h"

#include <stdint.h>

namespace iposix{
	namespace scheduler{
		namespace policies{
			

		class priority_round_robin
		{
			struct process_list
			{
				process_list* next;
				process_type* process;
			};

			private:
				process_list* ready_head;

				process_list* blocked_head;

				process_list* cur;	
			
			
			protected:
				priority_round_robin():
					ready_head( 0 ),
					blocked_head( 0 ),
					cur( 0 )
				{}

				virtual ~priority_round_robin()
				{}
			
			public:


				process_type* get_current_process() const
				{
					if( this->cur )
					{
						return this->cur->process;
					}
					else
					{
						return 0;
					}
				}

				process_type* get_next_process()
				{
					if( this->cur )
					{
						if ( this->cur->next)
						{
							this->cur=this->cur->next;
						}
						else
						{
							this->cur=this->ready_head;
						}
					
						if( this->cur )
						{
							return this->cur->process;
						}
					}
					
					return 0;
				}

				void push_new( process_type* process)
				{
					process_list* pl=new process_list();
					
					if(pl)
					{
						pl->process=process;
						
						pl->next=this->ready_head;

						if(!this->cur)
						{
							if(this->ready_head)
							{
								this->cur=this->ready_head;
							}
							else
							{
								this->cur=pl;
							}
						}
						this->ready_head=pl;
					}
					else
					{
						//New process has not been pushed.
					}
				}

				process_type* remove( process_type* process, process_list* list_head)
				{ 
					process_list* cur = this->list_head;
					process_list* prev=0;
					
					while ( cur && ( cur->process == process ) )
					{	
						prev=cur;
						cur=cur->next;				
						if ( cur && ( cur->process==process ) )
						{
							if ( prev )
							{
								prev->nect = cur->next;
							}  	
							else if ( cur == this->list_head )
							{
								this->list_head
							}
							else
							{
								//not found
							}
							
							process_type* p = cur->process;

							cur->process = 0;

							delete cur;
				
							return p;
						}
						
						return 0;
					}
					
					void block_process( )
					{
						process_list* pl = new process_list();
						
						if ( this->cur )
						{
							pl->process = this->cur->process;

							pl->next = this->blocked_head;

							this->blocked_head = pl;
						}
						else
						{
							//no current process?
						}		
					}
			
					void unblock_process( process_type* process )
					{
						process_list* pl = new process_list();
						
						pl->process = process;
				
						if ( pl )
						{
							pl->next = this->ready_head;

							if ( !this->cur )
							{
								if ( this->ready_head )
								{
									this->cur = this->ready_head;
								}
								else
								{
									this->cur = pl;
								}
								
							this->ready_head = pl;	

							}
						}
						else
						{
							//no process
						}
	
					}
					static inline uint64_t ticks_between_reschedulings()
					{
						return 1000;
					}
		}//policies
	}//scheduler
}//iposix
#endif
