#ifndef _KERN_INCLUDE_FS_I_FS_NODE_H_
#define _KERN_INCLUDE_FS_I_FS_NODE_H_

#include "utils/noncopyable.h"

#include <string>
#include <memory>
#include <map>
#include <sys/stat.h>
#include <stdint.h>


namespace iposix {
namespace fs {

/* Predefine */
class i_fs_node;

/** A shared pointer to an abstract filesystem node */
typedef ::std::shared_ptr< ::iposix::fs::i_fs_node > i_fs_node_ptr;

/**
 * Describes an abstract filesystem node
 */
class i_fs_node
	: virtual public ::iposix::utils::Noncopyable
{
	public:

		/**
		 * Constructor
		 * @param[in] path the pathname of this node
		 * @param[in] is_fs_root is this a filesystem root
		 * @param[in] is_directory is this a directory
		 * @param[in] is_tty is this a character device
		 */
		i_fs_node(
				const ::std::string& path,
				bool is_fs_root,
				bool is_directory,
				bool is_tty
				);

		/** Destructor */
		virtual ~i_fs_node();
		
		// interface


		/**
		 * Reads the content of a file node
		 * @param[in] offset where to start reading
		 * @param[in,out] buffer where to write read data
		 * @param[in] length the amount of data to read (in bytes)
		 */
		virtual void read( 
				const uint64_t offset, 
				void* buffer, 
				const uint32_t length ) = 0;

		/**
		 * Writes the content of a file node
		 * @param[in] offset where to start reading
		 * @param[in,out] buffer where to read data to write from
		 * @param[in] length the amount of data to read (in bytes)
		 */
		virtual void write( 
				const uint64_t offset, 
				void* buffer, 
				const uint32_t length ) = 0;

		/*
		 * Reads the content of a directory node
		 * @param[in] entry which entry should be read
		 * @param[in,out] buffer where to write read data
		 */
		 /*
		virtual void read_dir( 
				const uint32_t entry, 
				void* buffer ) = 0;
				*/

		/**
		 * Returns a subnode of this node (Important only direct subnode).
		 * @param[in] fullname the complete name of the node to create
		 * @param[in] subname the name of the node (subentry) to find
		 * @return a pointer to the node
		 */
		virtual i_fs_node* open_subnode(
				const ::std::string& fullname,
				const ::std::string& subname ) = 0;

		/**
		 * Dumps all subnodes to the logger.
		 */
		virtual void dump_subnodes() const = 0;

		/**
		 * Returns the length of this file
		 * @return the length
		 */
		virtual uint64_t length() const = 0;

		/**
		 * Returns the stat struct for this node
		 * @return the stat struct
		 */
		virtual struct stat stat() const = 0;

		// ~interface

		/** Counter for the uses (opens on this node) */
		uint32_t uses;

		/** the path to this node */
		const ::std::string path;

		/** Is this node the filesystems root node */
		const bool is_fs_root;

		/** Is this node a directory node */
		const bool is_directory;

		/** Is this node a tty */
		const bool is_tty;
};


} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_I_FS_NODE_H_ */
