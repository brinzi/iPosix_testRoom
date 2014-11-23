#include "fs/dev_fs_node.h"
#include "devices/i_block_device.h"
#include "devices/i_char_device.h"
#include "logger/logger.h"

namespace iposix {
namespace fs {

dev_fs_node::dev_fs_node(
				const ::std::string& name,
				const bool fs_root
				)
	: i_fs_node( name, fs_root, true, false ),
	subnodes(),
	node_type( pseudo_node ),
	drive(),
	first_address( 0 ),
	last_address( 0 ),
	console()
{ }

dev_fs_node::dev_fs_node(
		const ::std::string& name,
		i_block_device_ptr drive,
		uint64_t first_address,
		uint64_t last_address
		)
	: i_fs_node( name, false, false, false ),
	subnodes(),
	node_type( block_node ),
	drive( drive ),
	first_address( first_address ),
	last_address( last_address ),
	console()
{ }

dev_fs_node::dev_fs_node(
		const ::std::string& name,
		i_char_device_ptr console
		)
	: i_fs_node( name, false, false, true ),
	subnodes(),
	node_type( char_node ),
	drive(),
	first_address( 0 ),
	last_address( 0 ),
	console( console )
{ }

dev_fs_node::dev_fs_node(
		const ::std::string& path,
		const dev_fs_node& dev_node
		)
	: i_fs_node( path, dev_node.is_fs_root, dev_node.is_directory, dev_node.is_tty ),
	subnodes( dev_node.subnodes ),
	node_type( dev_node.node_type ),
	drive( dev_node.drive ),
	first_address( dev_node.first_address ),
	last_address( dev_node.last_address ),
	console( dev_node.console )
{ }


dev_fs_node::~dev_fs_node()
{ }

void dev_fs_node::add_subnode( dev_fs_node_ptr subnode )
{
	if ( !this->is_directory )
	{
		//TODO throw exception
		return;
	}

	if ( subnode )
	{
		this->subnodes[subnode->path] = subnode;
	}
}

void dev_fs_node::read(
		const uint64_t offset, 
		void* buffer, 
		const uint32_t length )
{
	if ( this->is_directory )
	{
		//TODO throw exception
		return;
	}

	switch ( this->node_type )
	{
		case pseudo_node:
			//TODO throw exception
			break;

		case block_node:
			{
				uint64_t base = this->first_address + offset;
				uint64_t up = this->first_address + offset + length;

				//check bounds
				if ( 
						( base < up ) && ( up <= this->last_address ) 
				   )
				{
					this->drive->read( base , buffer, length );
				}
			}
			break;

		case char_node:
			this->console->read( buffer, length );
			break;
	}
}

void dev_fs_node::write( 
		const uint64_t offset, 
		void* buffer, 
		const uint32_t length )
{
	if ( this->is_directory )
	{
		//TODO throw exception
		return;
	}

	switch ( this->node_type )
	{
		case pseudo_node:
			//TODO throw exception
			break;

		case block_node:
			{
				uint64_t base = this->first_address + offset;
				uint64_t up = this->first_address + offset + length;

				//check bounds
				if ( 
						( base < up ) && ( up <= this->last_address ) 
				   )
				{
					this->drive->write( base , buffer, length );
				}
			}
			break;

		case char_node:
			this->console->write( buffer, length );
			break;
	}
}

i_fs_node* dev_fs_node::open_subnode(
		const ::std::string& fullname,
		const ::std::string& subname )
{
	if ( this->is_directory )
	{
		dev_fs_node_ptr to_copy = this->subnodes.find(subname)->second;

		if ( to_copy )
		{
			logger << message::MORE_VERBOSE << __func__
				<< ": dev_fs_node found a subnode" << endl;

			return new dev_fs_node( fullname, *to_copy );
		}
	}
	else
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": dev_fs_node is not a directory" << endl;
	}

	return 0;
}

void dev_fs_node::dump_subnodes() const
{
	if ( !this->is_directory )
	{
		return;
	}

	logger << message::MORE_VERBOSE << __func__ << "Dumping subnode" << endl;

	for ( auto it = this->subnodes.begin(); it != this->subnodes.end(); it++ )
	{
		logger << '\t' << it->first.c_str() << endl;
	}
}

uint64_t dev_fs_node::length() const
{
	switch ( this->node_type )
	{
		case pseudo_node:
			return 0;

		case block_node:
			return last_address - first_address;
		
		case char_node:
			return 0;

		default:
			return 0;
	}
}

struct stat dev_fs_node::stat() const
{
	struct stat s;

	s.st_dev = 0; //TODO system for device ids
	s.st_ino = 0; //TODO Should be uniq may be a good idea
	s.st_mode = S_IRWXU | S_IRWXG | S_IRWXO;

	if ( this->is_directory )
	{
		s.st_mode |= S_IFDIR;
	}
	else
	{
		s.st_mode |= S_IFREG;
	}

	s.st_nlink = 1; //easy there is no linking implemented til now
	s.st_uid = 0;
	s.st_gid = 0;
	s.st_rdev = 1; //set 1 cause this is a special device
	s.st_size = this->last_address - this->first_address;
	s.st_atime = 0; //TODO system uptime?
	s.st_mtime = 0; //TODO system uptime?
	s.st_ctime = 0; //TODO system uptime?
	s.st_blksize = 512; //TODO
	s.st_blocks = s.st_size / s.st_blksize;

	return s;
}

} //namespace fs
} //namespace iposix
