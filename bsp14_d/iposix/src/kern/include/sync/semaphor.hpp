#ifndef _SEMAPHORE_
#define _SEMAPHORE_

#include "/semaphorei_process.h"

namespace iposix{
	namespace sync{

		template <typename _signal, typename _process>
		class semaphore
		{

			private:
				int signal;
				process_type* p;
								
			public:
			semaphore(_signal s, _process q) q):signal(s),
			p(q){};
			~semaphore();
		
			void p(void);
			void v(void);
		};

		void semaphore::p(void){

			(this->signal)--;
			if(this->signal<0){
				strategy->block();
			}


		};
		void semaphore::v(void){
			(this->signal)++;
			if(this->signal<=0){
				strategy->deblock(p);
			}
		}


		template class semaphore<SEMAPHORE>;
		typedef semaphore<SEMAPHORE> semaphore_type;
		}

}
#endif 