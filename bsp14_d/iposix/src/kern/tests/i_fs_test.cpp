#include "tests/tests.h"
#include "fs/i_fs.h"
#include "fs/fat_fs.h"
#include "fs/dev_fs.h"
#include "fs/dev_fs_node.h"
#include "x86/core/interrupts.h"
#include "logger/logger.h"

using namespace ::iposix::fs;

namespace iposix {
namespace tests {

bool mount_test()
{
	//mount the dummy root node
	//i_fs::mount_root_dummy();

	//mount dev_fs
	//i_fs::mount( fs_dev_fs, "/dev" );

	::iposix::utils::Singleton< dev_fs >::instance().root_node->dump_subnodes();

	//mount hda1
	i_fs::mount( fs_fat16_fs, "/", "/dev/hda1" );

	return true;
}

bool i_fs_test()
{
	#define assert( boolean ) if ( ! boolean ) return false

	#define test_open( str ) \
	{ \
		logger << message::MORE_VERBOSE << __func__ \
			<< ": opening " << str << endl; \
		i_fs_node_ptr tmp_node = i_fs::open_node( str ); \
		assert( tmp_node ); \
		i_fs::close_node( tmp_node ); \
	}

	test_open( "/dev" );
	test_open( "/dev/hda1" );
	test_open( "/boot/./././grub/../grub/iso9660_" );

	logger << message::MORE_VERBOSE << __func__
		<< ": Reading /boot/./././grub/../grub/menu.lst" << endl;
	{
		i_fs_node_ptr tmp_node = i_fs::open_node( "/boot/./././grub/../grub/menu.lst" );
		assert( tmp_node );

		if ( tmp_node )
		{
			char buffer[512];

			for ( uint32_t i = 0; i < 512; ++i )
			{
				buffer[i] = 0;
			}

			tmp_node->read( 0, &buffer[0], 200 );

			logger << message::MORE_VERBOSE << "Content " << &buffer[0] << endl;
		}
		else
		{
			return false;
		}
		i_fs::close_node( tmp_node );
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Reading /bla" << endl;
	{
		i_fs_node_ptr tmp_node = i_fs::open_node( "/bla");
		assert( tmp_node );

		if ( tmp_node )
		{
			char buffer[512];

			for ( uint32_t i = 0; i < 512; ++i )
			{
				buffer[i] = 0;
			}

			tmp_node->read( 0, &buffer[0], 200 );

			logger << message::MORE_VERBOSE << "Content " << &buffer[0] << endl;
		}
		else
		{
			return false;
		}
		i_fs::close_node( tmp_node );
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Writing a-z on stdout" << endl;
	{
		i_fs_node_ptr tmp_node = i_fs::open_node( "/dev/stdout" );
		for ( char i = 'a'; i <= 'z' ; i++)
		{
			char buffer[1];
			buffer[0] = i;
			tmp_node->write( 0, &buffer[0], 1 );
		}
		char end[] = "\n";
		tmp_node->write( 0, &end[0], 1 );
		i_fs::close_node( tmp_node );
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Writing stdin on stdout" << endl;
	logger << message::MORE_VERBOSE << __func__
		<< ": Switching interrupts on" << endl;
	::iposix::arch::x86::interrupt::interrupts_on();
	{
		i_fs_node_ptr input = i_fs::open_node( "/dev/stdin" );
		assert( input );

		i_fs_node_ptr output = i_fs::open_node( "/dev/stdout" );
		assert( output );

		char msg[] = "Please write something ('q' to end)\n";
		char buf[2];

		output->write( 0, &msg[0], sizeof( msg ) );

		while ( buf[0] != 'q' )
		{
			input->read( 0, &buf[0], 1 );
			output->write( 0, &buf[0], 1 );
		}
	}
	logger << message::MORE_VERBOSE << __func__
		<< ": Switching interrupts off" << endl;
	::iposix::arch::x86::interrupt::interrupts_off();

	return true;
}

} //namespace tests
} //namespace iposix
