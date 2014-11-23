#ifndef _KERN_INCLUDE_FS_I_FS_H_
#define _KERN_INCLUDE_FS_I_FS_H_

#include "utils/noncopyable.h"

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace iposix {
/**
 * Contains the abstract filesystem (i_fs).
 */
namespace fs {

/* Predefine */
class i_fs;
class i_fs_node;

/** A shared pointer to an abstract filesystem */
typedef ::std::shared_ptr< i_fs > i_fs_ptr;

/** All mountable filesystems */
enum i_fs_type_t
{
	/** The device filesystem */
	fs_dev_fs,

	/** A fat 12 filesystem */
	fs_fat12_fs,

	/** A fat 16 filesystem */
	fs_fat16_fs,

	/** A fat 32 filesystem */
	fs_fat32_fs
};

/**
 * An abstract filesystem. It also contains an overlay (mount table).
 */
class i_fs
	: virtual public ::iposix::utils::Noncopyable
{
	protected:
		/** A shared pointer to a abstract filesystem node */
		typedef ::std::shared_ptr< i_fs_node > i_fs_node_ptr;

		/**
		 * Constructor. This Constructs a filesystem interface.
		 * @param[in] container the node which contains this filesystem
		 */
		i_fs( i_fs_node_ptr container );

		/**
		 * Destructor
		 */
		virtual ~i_fs();

		/**
		 * Executes a raw read on the filesystem.
		 * @param[in] offset the offset to the first sector of this filesystem
		 * @param[in,out] buffer the buffer which should be filled with the
		 * readen data
		 * @param[in] length the length length of the buffer (the amount of
		 * data) in bytes
		 * \todo TODO throw exception
		 */
		void read(
				const uint64_t offset,
				void* buffer,
				const uint32_t length);

		/**
		 * Executes a raw write on the filesystem.
		 * @param[in] offset the offset to the first sector of this filesystem
		 * @param[in,out] buffer the buffer which holds the data to write
		 * @param[in] length the length length of the buffer (the amount of
		 * data) in bytes
		 * \todo TODO throw exception
		 */
		void write(
				const uint64_t offset, 
				void* buffer, 
				const uint32_t length);


	public:
		/**
		 * Return the root node
		 * @return the root node
		 */
		virtual i_fs_node* fs_root_node( const ::std::string& path ) = 0;

		/**
		 * Initilizes this filesystem
		 */
		virtual void init() = 0;

		/**
		 * Synchronizes this filesystem
		 */
		virtual void sync() = 0;

		// mounting

		/**
		 * Mount a filesystem with a specific type at specific location
		 * (mountpoint)
		 * @param[in] i_fs_type the type of the filesystem
		 * @param[in] mountpoint the location to mount at
		 * @param[in] nodename the pathname of the filesystem container
		 */
		static void mount(
				i_fs_type_t i_fs_type,
				const ::std::string& mountpoint,
				const ::std::string& nodename = "" );

		/**
		 * Mounts a dummy root node. This is needed unless there is a real root
		 * filesystem. Without this you cannot even reach nodes in the device
		 * filesystem and therefore it is not possible to mount a filesystem
		 */
		static void mount_root_dummy();

		/**
		 * Unmount a mountpoint
		 * @param[in] mount_point the mountpoint to unmount
		 */
		static void umount( const ::std::string& mount_point );

		// ~mounting

		// node_operations

		/**
		 * Opens a node by pathname
		 * \todo TODO currently only absoult pathnames are allowed
		 * \todo TODO make openflags (like create) available
		 * @param[in] name the pathname to the node
		 * @return the opened node (or a nullpointer if there is no)
		 */
		static i_fs_node_ptr open_node( 
				const ::std::string& name );

		/**
		 * Closes (or decreases the use count) a node
		 * @param[in] node the node to close
		 */
		static void close_node( i_fs_node_ptr node );

		// ~node_operations

		/**
		 * Initialzies the virtual filesystem (cleans arrays)
		 */
		static void i_fs_init();

		/**
		 * Dumps all mountpoints and opennodes
		 */
		static void i_fs_dump();

	private:

		/** The underlying container (normally a dev_fs_node) */
		i_fs_node_ptr container;

		/** a map between pathnames and filesystem root nodes (mount map) */
		static ::std::map< ::std::string, i_fs_node_ptr > mounted_filesystems;

		/** a map between pathnames and filenodes (open map) */
		static ::std::map< ::std::string, i_fs_node_ptr > open_nodes;

		/**
		 * Cleans and tokenizes the pathname
		 * @param[in] path the pathname to clean
		 * @return a array of _good_ tokens
		 */
		static ::std::vector< ::std::string > clean_path( const ::std::string& path );

		/**
		 * Marks a i_fs_node opened (or increases the count )
		 * @param[in] node the node to open
		 */
		static void open( i_fs_node_ptr node );

		/**
		 * Mounts a filesystem container of a specific type at a location (add
		 * to mount map)
		 * @param[in] i_fs_type the type of filesystem
		 * @param[in] mountpoint pathname or location to mount at
		 * @param[in] container the node which contains the filesystem
		 */
		static void mount( 
				i_fs_type_t i_fs_type,
				const ::std::string& mountpoint,
				i_fs_node_ptr container );

		/**
		 * Helper function to recurse in to the filesystem hierachie
		 * @param[in] node the base node to search for subnodes
		 * @param[in] fullname the fullname to the node
		 * @param[in] subnode_name the name of the subnode to search for
		 * @return the result node (or null pointer if there is no)
		 */
		static i_fs_node_ptr open_subnode(
				i_fs_node_ptr node,
				const ::std::string& fullname,
				const ::std::string& subnode_name );
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_I_FS_H_ */
