#ifndef _KERN_INCLUDE_FS_DEV_FS_NODE_H_
#define _KERN_INCLUDE_FS_DEV_FS_NODE_H_

#include "fs/i_fs_node.h"
#include "utils/noncopyable.h"

#include <map>
#include <string>
#include <stdint.h>

namespace iposix {

/* Predefine */
namespace arch {
class i_block_device;
class i_char_device;
} //namespace arch

/**
 * Contains the device filesystems nodes.
 */
namespace fs {

/**
 * The type of a device filesystem node.
 */
enum dev_fs_node_type_t
{
	/** Is this a pseudo node (aka directory) */
	pseudo_node,
	/** Is this a drive node */
	block_node,
	/** Is this a console node (stdin, stderr, stdout) */
	char_node
	// add more here
};

/* Predefine */
class dev_fs_node;

/**
 * A shared pointer to a device filesystem node.
 */
typedef ::std::shared_ptr< ::iposix::fs::dev_fs_node > dev_fs_node_ptr;

/**
 * This describes a node (file) hold by the device filesystem. These nodes
 * prepent to be file and therefore allows to access a device like a file.
 * \todo TODO implement good version of stat()
 */
class dev_fs_node
	: virtual public ::iposix::utils::Noncopyable,
	public i_fs_node
{
	private:
		/**
		 * A shared pointer to an abstract drive
		 */
		typedef ::std::shared_ptr< ::iposix::arch::i_block_device > i_block_device_ptr;
		typedef ::std::shared_ptr< ::iposix::arch::i_char_device > i_char_device_ptr;

	public:
		/**
		 * Constructor for a directory node which containts subnodes.
		 * @param[in] name the name of the node
		 * @param[in] fs_root is this the root node (true for the root_node)
		 */
		dev_fs_node(
				const ::std::string& name,
				const bool fs_root
				);

		/**
		 * Constructor for a device filesystem node with a underlying abstract
		 * drive.
		 * @param[in] name the name of the node
		 * @param[in] drive a pointer to the underlying drive
		 * @param[in] first_address the first accessible address on the drive
		 * @param[in] last_address the last accessible address on the drive
		 */
		dev_fs_node(
				const ::std::string& name,
				i_block_device_ptr drive,
				uint64_t first_address,
				uint64_t last_address
				);

		/**
		 * Constructor for ad device filesystem node with a underlying console.
		 * @param[in] name the name of the node
		 * @param[in] console a pointer to the underlying console
		 */
		dev_fs_node(
				const ::std::string& name,
				i_char_device_ptr console
				);

		/**
		 * Copy-Constructor used for copying internal nodes to the outside
		 * @param[in] path the path which the nodes has to report on requests
		 * @param[in] dev_node the node to copy
		 */
		dev_fs_node(
				const ::std::string& path,
				const dev_fs_node& dev_node
				);

		/**
		 * Destructor
		 */
		virtual ~dev_fs_node();

		/**
		 * Adds a subnode to this node (only applicable to directory nodes)
		 * @param[in] subnode the subnode which should be add
		 */
		void add_subnode( dev_fs_node_ptr subnode );

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


		//~i_fs_node

	private:
		/** A map of subnodes */
		::std::map< ::std::string, dev_fs_node_ptr > subnodes;

		/** The type of this node */
		const dev_fs_node_type_t node_type;

		// for drive_nodes

		/** The underlying drive */
		i_block_device_ptr drive;

		/** The first accessible address on the drive */
		const uint64_t first_address;

		/** The last accessible address on the drive */
		const uint64_t last_address;

		//~for drive_nodes

		// for console_nodes

		/** The underlying console */
		i_char_device_ptr console;

		// ~for console_nodes
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_DEV_FS_NODE_H_ */
