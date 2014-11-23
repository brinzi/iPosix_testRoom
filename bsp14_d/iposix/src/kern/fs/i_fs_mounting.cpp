#include "fs/i_fs.h"
#include "fs/i_fs_node.h"
#include "fs/i_fs_dummy_node.h"
#include "fs/dev_fs.h"
#include "fs/fat_fs.h"
#include "logger/logger.h"

#include <string>

namespace iposix {
namespace fs {

::std::map< ::std::string, i_fs_node_ptr > i_fs::mounted_filesystems;

void i_fs::mount( i_fs_type_t i_fs_type,
		const ::std::string& mountpoint,
		i_fs_node_ptr container )
{
	//!!!: here we use a real pointer to the new filesystem. since we do not
	//want it to be destroyed. this is because if we create it here (on the
	//stack or as a shared pointer) the filesystem will be destroyed after
	//returning from this method and this simple should not happen because the
	//depending root node (maybe) needs the filesystem. And because of
	//filesystems should be implemented with shared_pointers these pointers
	//should destroy the filesystem after every node which belongs to this
	//filesystem is destroyed.
	i_fs* new_fs = 0;

	logger << message::MORE_VERBOSE << __func__;

	switch ( i_fs_type )
	{
		case fs_dev_fs:
			logger << ": mount dev_fs at ";
			new_fs = &::iposix::utils::Singleton< dev_fs >::instance();
			break;

		case fs_fat12_fs:
			logger << ": mount fat12_fs at ";
			new_fs = new fat_fs( container, fat12 );
			break;

		case fs_fat16_fs:
			logger << ": mount fat16_fs at ";
			new_fs = new fat_fs( container, fat16 );
			break;

		case fs_fat32_fs:
			logger << ": mount fat32_fs at ";
			new_fs = new fat_fs( container, fat32 );
			break;

		default:
			break;
	}

	logger << mountpoint.c_str() << endl;

	if ( new_fs )
	{
		new_fs->init();
		i_fs_node_ptr root_node( new_fs->fs_root_node( mountpoint ) );

		if ( root_node )
		{
			i_fs::mounted_filesystems[root_node->path] = root_node;
		}
	}
}

void i_fs::mount(
		i_fs_type_t i_fs_type,
		const ::std::string& mountpoint,
		const ::std::string& nodename )
{
	i_fs_node_ptr node;

	if ( !nodename.empty() )
	{
		node = i_fs::open_node( nodename );
	}

	if ( node || (i_fs_type == fs_dev_fs) )
	{
		i_fs::mount( i_fs_type, mountpoint, node );
	}
}

void i_fs::mount_root_dummy()
{
	if ( !i_fs::mounted_filesystems.find( "/" )->second )
	{	
		i_fs_node* root_dummy = new i_fs_dummy_node();
		i_fs_node_ptr ptr = i_fs_node_ptr( root_dummy );
		i_fs::mounted_filesystems["/"] = ptr;
	}
}

void i_fs::umount( const ::std::string& mountpoint )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Unmounting " << mountpoint.c_str() << endl;

	//TODO check open nodes
	i_fs::mounted_filesystems.erase( mountpoint );
}

} //namespace fs
} //namespace iposix
