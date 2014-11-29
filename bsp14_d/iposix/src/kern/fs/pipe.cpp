#ifndef _CLASS_PIPE_
#define _CLASS_PIPE_

#include <stdint.h>
#include "fs/filehandle.h"

namespace iposix{
	namespace fs{
		class pipe{
		
			private:
				//the buffer where the data is going to be stored.
				char* buffer;
				//the filehandle 
				filehandle_ptr fd;		
				//the two filedescriptors 
				int in, out;
				//theese represent if the streams are closed or open
				bool instream, outstream;
					
			public:
				/**Constructor*/
				pipe( int fd[])
				{
					this->in = fd[0];

					this->out = fd[1];
				}
				/**Destructor*/
				~pipe()
				{}
			
				/**Read function*/
				uint32_t read()
				{
					if ( instream )
					{	

						fd = cur_process->get_filehandle( in );
							
						uint32_t len = fd->read( this->buffer, 512 );
				
						return len;
					}
					else return 0;	
				}

				/**Write function*/				
				uint32_t write()
				{	
					if ( outstream )
					{	
						fd = cur_process->get_filehandle( out );
						
						uint32_t len = fdout->write( this->buffer ,len );
					
						return len;
					}
					else return 0;
				}
				/**Switch the input stream status*/
				void change_read()
				{
					instream = !instream;
				}
				/**Changes the output stream status*/
				void change_write()
				{
					outstream = !outstream;
				}
		}//pipe
	}//fs
}//iposix				
#endif 
