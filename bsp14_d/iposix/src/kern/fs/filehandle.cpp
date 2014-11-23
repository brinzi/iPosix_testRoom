#include "fs/filehandle.h"
#include "fs/i_fs_node.h"

#include <unistd.h>
#include <errno.h>

namespace iposix {
namespace fs {

filehandle::filehandle(
		i_fs_node_ptr node,
		const char flags,
		const char mode,
		const uint32_t filehandle_num
		)
	: node( node ),
	mode( mode ),
	flags( flags ),
	fd( filehandle_num ),
	offset( 0 )
{ }

filehandle::~filehandle()
{ }

uint32_t filehandle::read( void* buffer, uint32_t length )
{	auto result=0;
	if ( this->node )
	{
		this->node->read( this->offset, buffer, length );
		result=this->offset += length;
	}

	return result;
}

uint32_t filehandle::write( void* buffer, uint32_t length )
{	
	auto result=0;
	if ( this->node )
	{
		this->node->write( this->offset, buffer, length );
		result=this->offset += length;
	}

	return result;
}


bool filehandle::seek( int64_t offset, int32_t direction )
{
	if ( this->node )
	{
		if ( this->node->is_tty )
		{
			return EBADF;
		}

		switch ( direction )
		{
			case SEEK_SET:
				if (
						( offset < static_cast<int64_t>( this->node->length() ) ) &&
						( offset >= 0 )
				   )
				{
					this->offset = offset;
				}
				else
				{
					return ESPIPE;
				}
				break;

			case SEEK_CUR:
				if (
						( this->offset + offset < static_cast<int64_t>( this->node->length() ) ) &&
						( this->offset + offset >= 0 )
				   )
				{
					this->offset += offset;
				}
				else
				{
					return ESPIPE;
				}
				break;

			case SEEK_END:
				if (
						( static_cast<int64_t>( this->node->length() ) + offset < static_cast<int64_t>( this->node->length() ) ) &&
						( static_cast<int64_t>( this->node->length() ) + offset >= 0 )
				   )
				{
					this->offset = static_cast<int64_t>( this->node->length() ) + offset;
				}
				else
				{
					return ESPIPE;
				}
				break;
		}
	}

	return 0;
}

bool filehandle::stat( struct stat* st )
{
	if ( this->node )
	{
		*st = this->node->stat();	
		return true;
	}

	return false;
}

} //namespace fs
} //namespace iposix
