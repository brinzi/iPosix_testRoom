#ifndef _KERN_INCLUDE_FS_I_FS_DUMMY_NODE_H_
#define _KERN_INCLUDE_FS_I_FS_DUMMY_NODE_H_

#include "fs/i_fs_node.h"
#include "utils/noncopyable.h"

namespace iposix {

/**
 * Contains the a dummy node
 */
namespace fs {


/**
 * This describes a dummy node. Should only be used as the root "/" unless there
 * is no alternative mounted.
 */
class i_fs_dummy_node
	: virtual public ::iposix::utils::Noncopyable,
	public i_fs_node
{
	public:
		/**
		 * Constructor
		 */
		i_fs_dummy_node(
				const ::std::string& path = "/",
				bool fs_root = true,
				bool directory = true );

		/**
		 * Destructor
		 */
		virtual ~i_fs_dummy_node();

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
		 * Returns a subnode of this node
		 * @param[in] path full path to the node
		 * @param[in] name the name of the node to find
		 * @return a pointer to the node
		 */
		i_fs_node* open_subnode(
			const ::std::string& path,
			const ::std::string& name );

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
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_I_FS_DUMMY_NODE_H_ */
