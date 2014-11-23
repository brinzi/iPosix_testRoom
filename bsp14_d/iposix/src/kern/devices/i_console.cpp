#include "devices/i_console.h"
#include "iposix/iposix_kernel.h"
#include "fs/dev_fs.h"
#include "fs/dev_fs_node.h"

#include <string>

namespace iposix
{
namespace arch
{

using ::iposix::fs::dev_fs;
using ::iposix::fs::dev_fs_node;
using ::iposix::fs::dev_fs_node_ptr;

i_console::i_console( const ::std::string& name )
	: i_char_device( name )
{ }

i_console::~i_console()
{ }

uint32_t i_console::write( const void* buffer, uint32_t length )
{
	const char* buf = reinterpret_cast<const char*>( buffer );
	for ( uint32_t i = 0; i < length; ++i )
	{
		::iposix::utils::Singleton< kernel >::instance().video.write( buf[i] );
	}
	return length;
}

uint32_t i_console::read( void* buffer, uint32_t /*length*/ )
{
	char* buf = reinterpret_cast<char*>( buffer );

	buf[0] = ::iposix::utils::Singleton< kernel >::instance().keyboard.get_next_key();

	return 1;
}

void i_console::clear()
{
	::iposix::utils::Singleton< kernel >::instance().video.clear();
}

void i_console::init()
{
	static const ::std::string STDIN = "stdin";
	static const ::std::string STDERR = "stderr";
	static const ::std::string STDOUT = "stdout";

	//create a console
	i_char_device_ptr console = i_char_device_ptr();

	//get the dev_fs root node
	dev_fs_node_ptr root = ::iposix::utils::Singleton< ::iposix::fs::dev_fs >::instance().root_node;

	//add the node as stdin, stderr, stdout
	console = i_char_device_ptr( new i_console( STDIN ) );
	dev_fs_node_ptr io = dev_fs_node_ptr( new dev_fs_node( STDIN, console ) );
	root->add_subnode( io );

	console = i_char_device_ptr( new i_console( STDERR ) );
	io = dev_fs_node_ptr( new dev_fs_node( STDERR, console ) );
	root->add_subnode( io );

	console = i_char_device_ptr( new i_console( STDOUT ) );
	io = dev_fs_node_ptr( new dev_fs_node( STDOUT, console ) );
	root->add_subnode( io );

}

} //namespace arch
} //namespace iposix
