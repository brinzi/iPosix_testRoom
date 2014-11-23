#include "fs/i_fs_node.h"
#include "fs/i_fs.h"
#include "logger/logger.h"

namespace iposix {
namespace fs {

i_fs_node::i_fs_node(
		const ::std::string& path,
		bool is_fs_root,
		bool is_directory,
		bool is_tty )
	: uses( 0 ),
	path( path ),
	is_fs_root( is_fs_root ),
	is_directory( is_directory ),
	is_tty( is_tty )
{ }

i_fs_node::~i_fs_node()
{ }

} //namespace fs
} //namespace iposix
