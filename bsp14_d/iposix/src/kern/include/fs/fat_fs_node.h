#ifndef _KERN_INCLUDE_FS_FAT_FS_NODE_H_
#define _KERN_INCLUDE_FS_FAT_FS_NODE_H_

#include "fs/i_fs_node.h"
#include "fs/fat_directory_entry.hpp"
#include "utils/noncopyable.h"

#include <memory>
#include <stdint.h>
#include <unistd.h>

namespace iposix {
/**
 * Contains the fat filesystems nodes
 */
namespace fs {

/* Predefine */
class fat_fs;
class fat_directory_entry_t;

/**
 * Describes a fat filesystems node (directory/file)
 * \todo TODO implement vfat extension (long names). May writing an intellegent
 * fat_directory_table.
 * \todo TODO dump_subnodes
 * \todo TODO create good version of stat()
 */
class fat_fs_node
	: virtual public ::iposix::utils::Noncopyable,
	public i_fs_node
{
	private:
		/** A shared pointer to a fat filesystem */
		typedef ::std::shared_ptr< ::iposix::fs::fat_fs > fat_fs_ptr;
		
	public:
		/**
		 * Constructor
		 * @param[in] fs the depending filesystem
		 * @param[in] directory is this node a directory
		 * @param[in] path the path under which the node should be visible
		 * @param[in] fs_root is this node the filesystems root node
		 * @param[in] cluster the first cluster of this node
		 * @param[in] entry the directory for this node
		 */
		fat_fs_node(
			fat_fs_ptr fs,
			const ::std::string& path,
			const bool directory,
			const bool fs_root,
			const uint32_t cluster,
			fat_directory_entry_t entry
			);

		/** Destructor */
		~fat_fs_node();

		/**
		 * Searchs for an entry in a directory node
		 * @param[in] name the name of the entry to be searched
		 * @param[in] entry the coresponding fat_entry
		 */
		bool entry_by_name( const ::std::string& name, fat_directory_entry_t* entry );

		// i_fs_node

		/**
		 * Reads the content of a file node
		 * @param[in] offset where to start reading
		 * @param[in,out] buffer where to write read data
		 * @param[in] length the amount of data to read (in bytes)
		 */
		void read( 
				const uint64_t offset, 
				void* buffer, 
				const uint32_t length );

		/**
		 * Writes the content of a file node
		 * @param[in] offset where to start reading
		 * @param[in,out] buffer where to read data to write from
		 * @param[in] length the amount of data to read (in bytes)
		 */
		void write( 
				const uint64_t offset, 
				void* buffer, 
				const uint32_t length );

		/**
		 * Reads the content of a directory node
		 * @param[in] entry which entry should be read
		 * @param[in,out] buffer where to write read data
		 */
		void read_dir( 
				const uint32_t entry, 
				void* buffer );

		/**
		 * Returns a subnode of this node (Important only direct subnode).
		 * @param[in] fullname the complete name of the node to create
		 * @param[in] subname the name of the node (subentry) to find
		 * @return a pointer to the node
		 */
		i_fs_node* open_subnode(
				const ::std::string& fullname,
				const ::std::string& subname );

		/**
		 * Dumps all subnodes to the logger.
		 */
		void dump_subnodes() const;

		/**
		 * Returns the length of this file
		 * @return the length
		 */
		uint64_t length() const;

		/**
		 * Returns the stat struct for this node
		 * @return the stat struct
		 */
		struct stat stat() const;

		// ~i_fs_node

	private:
		/** the filesystem which holds this node */
		fat_fs_ptr fs;

		/** the first cluster of this node */
		uint32_t cluster;

		/** The directory entry which describing this node */
		fat_directory_entry_t entry;

		/**
		 * helper function for reading
		 */
		inline void read_pure( uint64_t offset, void *buffer, uint32_t length );

		/**
		 * helper function for writing
		 */
		inline void write_pure( uint64_t offset, void *buffer, uint32_t length );
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_FS_NODE_H_ */
