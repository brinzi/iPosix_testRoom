#ifndef _CLASS_PIPE_
#define _CLASS_PIPE_

#include <stdint.h>
#include "fs/filehandle.h"

namespace iposix{
	namespace fs{
		class pipe{
		
			private:
				char* buffer;
				
				filehandle_ptr fd;		

			public:
			
				pipe( int fd[])
				{
					this->read_fd = fd[0];

					this->write_fd = fd[1];
				}

				~pipe()
				{}
			
				
				
				void set_read( int file_descriptor )
				{
					fd = cur_process->get_filhandle( file_descriptor );
				}

				uint32_t read()
				{
					uint32_t len = fd->read( this->buffer, 512 );
					return len;
				}
				
				void set_write()
				{
					fd = cur_process->get_filehandle( file_descriptor );
				}
				
				uint32_t write()
				{
					uint32_t len = filehandle::write( this->buffer ,len );
					
					return len;
				}
				
		}//pipe
	}//fs
}//iposix				
#endif 
