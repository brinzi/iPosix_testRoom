#include "fs/fat_fs_node.h"
#include "fs/fat_fs.h"
#include "logger/logger.h"

#include <cstring>


namespace iposix {
namespace fs {

fat_fs_node::fat_fs_node(
		fat_fs_ptr fs,
		const ::std::string& path,
		const bool directory,
		const bool fs_root,
		const uint32_t cluster,
		fat_directory_entry_t entry
		)
	: i_fs_node( path, fs_root, directory, false ),
	fs( fs ),
	cluster( cluster ),
	entry( entry )
{ 
}

fat_fs_node::~fat_fs_node()
{
}

void fat_fs_node::read( 
		uint64_t offset, 
		void* buffer, 
		uint32_t length )
{
	if ( this->is_directory )
	{
		//TODO throw exception
		return;
	}

	//setting length to maximum
	if ( ( offset + length ) > this->entry.size )
	{
		length = this->entry.size - offset;
	}

	this->read_pure( offset, buffer, length );
}

void fat_fs_node::read_pure(
		const uint64_t offset, 
		void* buffer, 
		const uint32_t length )
{
	this->fs->cluster_io( fat_fs::cluster_read, this->cluster, offset, buffer, length );
}

void fat_fs_node::write( 
		uint64_t offset, 
		void* buffer, 
		uint32_t length)
{
	if ( this->is_directory )
	{
		//TODO throw exception
		return;
	}

	//setting length to maximum
	if ( ( offset + length ) > this->entry.size )
	{
		length = this->entry.size - offset;
	}

	this->write_pure( offset, buffer, length );
}

void fat_fs_node::write_pure(
		const uint64_t offset, 
		void* buffer, 
		const uint32_t length )
{
	this->fs->cluster_io( fat_fs::cluster_write, this->cluster, offset, buffer, length );
}

void fat_fs_node::read_dir( 
		const uint32_t entry, 
		void* )
{
	if ( !this->is_directory )
	{
		//TODO throw exception
		return;
	}

	//uint32_t entry_length( sizeof( fat_directory_entry_t ) );
	uint32_t offset( entry * sizeof( fat_directory_entry_t ) );

	fat_directory_entry_t buf_entry;

	this->read_pure( offset, &buf_entry, sizeof( fat_directory_entry_t ) );

	//TODO handle entry not only dump it out
	buf_entry.dump();
}

bool fat_fs_node::entry_by_name( const ::std::string& name, fat_directory_entry_t* entry )
{
	if ( !this->is_directory )
	{
		return false;
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Searching for " << name.c_str() << endl;

	uint32_t clustersize = this->fs->cluster_size();
	
	uint32_t max_entrys( clustersize / sizeof( fat_directory_entry_t ) );

	fat_directory_entry_t* entrys = new fat_directory_entry_t[max_entrys]();

	uint32_t clusters = this->fs->clustertable.chain_length( this->cluster );

	logger << message::MORE_VERBOSE << __func__
		<< ": Chain length " << clusters << endl;

	uint64_t offset( 0 );
	bool found = false;
	for ( uint32_t i = 0; i < clusters; i++, offset += clustersize )
	{
		this->read_pure( offset, &entrys[0], clustersize );

		bool abort = false;
		for ( uint32_t j = 0; j < max_entrys; j++)
		{
			if ( entrys[j].attributes.volume_label )
			{
				logger << message::MORE_VERBOSE << __func__
					<< ": Skipping Volume label" << endl;
				continue;
			}

			if ( entrys[j].name[0] == '\0' )
			{
				//there will be no more entrys
				//throw exception;
				abort = true;
				break;
			}

			//get the name
			::std::string entry_name = entrys[j].filename();

			logger << message::MORE_VERBOSE << __func__ 
				<< ": Processing entry " << entry_name.c_str() << endl;

			if ( entry_name == name )
			{
				logger << message::MORE_VERBOSE << __func__ << " found" << endl;
				*entry = entrys[j];
				found = true;
				break;
			}
		}

		if ( abort || found )
		{
			break;
		}
	}

	delete[] entrys;

	if ( !found )
	{
		logger << message::MORE_VERBOSE << "not found" << endl;
		return false;
	}

	return true;
}

i_fs_node* fat_fs_node::open_subnode(
		const ::std::string& fullname,
		const ::std::string& subname )
{
	if ( !this->is_directory )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Not Directory while search for " << fullname.c_str()
			<< endl;
		//TODO throw exception
		return 0;
	}

	fat_directory_entry_t entry;
	bool success = this->entry_by_name( subname, &entry );
	if ( !success )
	{
		return 0;
	}

	entry.dump();

	bool directory( entry.attributes.directory ? true : false );
	uint32_t cluster( entry.cluster( this->fs->fat_type() ) );
	bool fs_root( cluster == 0 ); //root node has cluster 0
	uint32_t size = entry.size;

	if ( !fs_root )
	{
		cluster -= 2; //by fat definition the cluster number is + 2

		//we need to a a bit in case this is fat12/fat16
		cluster += this->fs->norm_node_cluster_add();
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Creating new fat_fs_node" 
		<< " on cluster: " << dec << cluster
		<< " with length: " << size
		<< " as " << ( directory ? "directory" : "file" )
		<< endl;

	return new fat_fs_node( this->fs, fullname, directory, fs_root, cluster, entry );
}

void fat_fs_node::dump_subnodes() const
{
	//TODO
}

uint64_t fat_fs_node::length() const
{
	return this->entry.size;
}

struct stat fat_fs_node::stat() const
{
	struct stat s;

	s.st_dev = 0; //TODO system for device ids
	s.st_ino = this->cluster; //Should be uniq may be a good idea
	s.st_mode = S_IRWXU | S_IRWXG | S_IRWXO;

	if ( this->entry.attributes.directory )
	{
		s.st_mode |= S_IFDIR;
	}
	else
	{
		s.st_mode |= S_IFREG;
	}

	s.st_nlink = 1; //easy there is no linking on fat filesystems
	s.st_uid = 0;
	s.st_gid = 0;
	s.st_rdev = 0; //set 0 cause this is a real device
	s.st_size = this->entry.size;
	s.st_atime = this->entry.access_time_unix();
	s.st_mtime = this->entry.modify_time_unix();
	s.st_ctime = this->entry.create_time_unix();
	s.st_blksize = this->fs->cluster_size();
	s.st_blocks = this->entry.size / this->fs->cluster_size();

	return s;
}

} //namespace fs
} //namespace iposix
