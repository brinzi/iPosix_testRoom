#include "fs/i_fs_dummy_node.h"
#include "logger/logger.h"

namespace iposix {
namespace fs {

i_fs_dummy_node::i_fs_dummy_node(
		const ::std::string& path,
		bool fs_root,
		bool directory )
	: i_fs_node( path, fs_root, directory, false )
{ }

i_fs_dummy_node::~i_fs_dummy_node()
{ }

void i_fs_dummy_node::read( const uint64_t, void*, const uint32_t )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Dummy node could not read" << endl;
}

void i_fs_dummy_node::write( const uint64_t, void*, const uint32_t )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": Dummy node could not write" << endl;
}

i_fs_node* i_fs_dummy_node::open_subnode(
		const ::std::string&,
		const ::std::string& )
{
	logger << message::MORE_VERBOSE << __func__
		<< ": dummy nodes does not have subnodes" << endl;

	return 0;
}

void i_fs_dummy_node::dump_subnodes() const
{
	logger << message::MORE_VERBOSE << __func__
		<< ": dummy nodes does not have subnodes" << endl;
}

uint64_t i_fs_dummy_node::length() const
{
	return 0;
}

struct stat i_fs_dummy_node::stat() const
{
	struct stat ret;
	
	logger << message::MORE_VERBOSE << __func__
		<< ": dummy nodes does not have stats" << endl;

	return ret;
}


} //namespace fs
} //namespace iposix
