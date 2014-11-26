#ifndef _CLASS_PIPE_
#define _CLASS_PIPE_

#include <stdint.h>
#include "fs/filehandle.h"

namespace iposix{
	namespace fs{
		class pipe{
		
			private:
				int read_fd;
				
				int write_fd;

				char* buffer;
			
			public:
			
				pipe( int fd[])
				{
					this->read_fd = fd[0];

					this->write_fd = fd[1];
				}

				~pipe()
				{}
			
				uint32_t read()
				{
					return filehandle::read( this->buffer , 512 )
				}

				void write(uint32_t len  )
				{
					filehandle::write( this->buffer ,len );
				}
				
				
		};	this->context_switch();
//fs
	}//iposix
}
#endif 
