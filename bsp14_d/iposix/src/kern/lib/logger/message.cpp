#include "message.h"
#include <stdint.h>

namespace iposix {
namespace logging {

message::message( const char * message, msg_level level, type message_type, identifiable * sender )
	: sender( sender ),
	  message_type( message_type ),
	  error( false ),
	  level( level ),
	  position( 0 )
{
	if( message == 0 )
	{
		this->msg[0] = '\0';
		return;
	}

	char * to = &this->msg[0];

	while( position < MSG_SIZE )
	{
		*to = *message;

		if( *message == '\0' )
		{
			break;
		}
		else
		{
			++to;
			++message;
			++position;
		}
	}

	if( position == MSG_SIZE )
	{
		error = true;
		this->msg[MSG_SIZE - 1] = '\0';
		return;
	}
}

}} //iposix::logging
