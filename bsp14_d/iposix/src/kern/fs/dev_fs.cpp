#include "fs/dev_fs.h"
#include "fs/dev_fs_node.h"

namespace iposix {
namespace fs {

dev_fs::dev_fs()
	: i_fs( i_fs_node_ptr() ),
	root_node( new dev_fs_node( "dev_root", true ) )
{ 
}

dev_fs::~dev_fs()
{ }

i_fs_node* dev_fs::fs_root_node( const ::std::string& path )
{
	return new dev_fs_node( path, *this->root_node );
}

void dev_fs::init()
{
	//nothing to do
}

void dev_fs::sync()
{
	//nothing to do
}


} //namespace fs
} //namespace iposix
