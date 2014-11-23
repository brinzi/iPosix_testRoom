#ifndef _KERN_INCLUDE_FS_DEV_FS_H_
#define _KERN_INCLUDE_FS_DEV_FS_H_

#include "fs/i_fs.h"
#include "utils/singleton.hpp"

#include <memory>
#include <stdint.h>

namespace iposix {

/**
 * This namespace contains the device filesystem and its nodes
 */
namespace fs {

/* Predefine */
class dev_fs_node;
class dev_fs;

/**
 * A shared Pointer to a device filesystem
 */
typedef ::std::shared_ptr< dev_fs > dev_fs_ptr;

/**
 * This describes a device filesystem which holds nodes which describes device
 * and pretend to be a file
 */
class dev_fs
	: virtual public ::iposix::utils::Singleton< dev_fs >,
	public i_fs
{
	friend class ::iposix::utils::Singleton< dev_fs >;

	private:
		/**
		 * A shared pointer to a device filesystem node.
		 */
		typedef ::std::shared_ptr< dev_fs_node > dev_fs_node_ptr;

	public:
		/**
		 * The root node of this filesystem.
		 */
		dev_fs_node_ptr root_node;

		// i_fs

		/**
		 * Return the root node.
		 * @return the root node
		 */
		i_fs_node* fs_root_node( const ::std::string& path );

		/**
		 * Initializes this filesystem.
		 */
		void init();

		/**
		 * Synchronizes this filesystem.
		 */
		void sync();

		// ~i_fs

	private:
		/** Constructor */
		dev_fs();

		/** Destructor */
		~dev_fs();

};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_DEV_FS_H_ */
