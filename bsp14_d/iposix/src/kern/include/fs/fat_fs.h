#ifndef _KERN_INCLUDE_FS_FAT_FS_H_
#define _KERN_INCLUDE_FS_FAT_FS_H_

#include "fs/i_fs.h"
#include "fs/fat_type.h"
#include "fs/fat_clustertable.h"
#include "fs/fat_bootsector.h"
#include "utils/noncopyable.h"

#include <memory>
#include <time.h>
#include <stdint.h>

namespace iposix {
/**
 * Contains the fat filesystems.
 */
namespace fs {

/* Predefine */
class fat_fs_node;

/**
 * Describes the fat filesystem
 * \todo TODO Handle Errors ( currently there is no error handling )
 * \todo TODO BUG in the root node: there is no successor cluster (chain length
 * = 1) which is defenitly wrong this is because fat12/fat16 handles the
 * filesystem root in a different way then any other directory.
 * FIX: in clustertable: if the requested cluster is 0 (i think) return a
 * different value. and in successor function retrun the physical successor
 * \todo TODO write support
 */
class fat_fs
	: public i_fs,
	virtual public ::iposix::utils::Noncopyable
{
	private:
		/** A shared pointer to a fat filesystem node */
		typedef ::std::shared_ptr< fat_fs_node > fat_fs_node_ptr;

	public:
		/**
		 * Constructor for a fat filesystem.
		 * @param[in] container the filesystem containing node (normaly a device
		 * node)
		 * @param[in] type the fat filesystem type
		 */

		fat_fs(
				i_fs_node_ptr container,
				fat_type_t type
				);

		/**
		 * Destructor
		 */
		~fat_fs();

		/** The clustertable */
		clustertable_t clustertable;

		/**
		 * Read a cluster from the filesystem
		 * @param[in] cluster the cluster to read
		 * @param[in] buffer where to write the read data
		 */
		void read_cluster( const uint32_t cluster, void* buffer );

		/**
		 * The types of I/O operations which could be performed
		 */
		enum fat_io_type_t
		{
			/** Read Operation */
			cluster_read,
			/** Write Operation */
			cluster_write
		};

		/**
		 * Performs I/O on a cluster chain.
		 * @param[in] io_type read or write?
		 * @param[in] start_cluster the cluster to perform I/O on
		 * @param[in] offset the offset to the start of the cluster
		 * @param[in] buffer the buffer to read/write from/to
		 * @param[in] length how mayn bytes should be read/write
		 */
		void cluster_io(
				fat_io_type_t io_type,
				uint32_t start_cluster,
				uint64_t offset,
				void* buffer,
				uint32_t length );

		/**
		 * Returns the cluster size
		 * @return the cluster size
		 */
		uint32_t cluster_size() const;

		/**
		 * Return the number of cluster which _musst_ be added to a normal node
		 * (except the root node). This abstracts from different handling
		 * between filesystem types.
		 * @return
		 */
		uint32_t norm_node_cluster_add() const;

		/**
		 * Returns the type of this filesystem
		 * @return the type of this filesystem
		 */
		fat_type_t fat_type() const;

		// i_fs

		/**
		 * Return the root node
		 * @return the root node
		 */
		i_fs_node* fs_root_node( const ::std::string& path );

		/**
		 * Initilizes this filesystem
		 */
		void init();

		/**
		 * Synchronises this filesystem with the underlyding node.
		 */
		void sync();

		// ~i_fs

	private:
		/** The type of this filesystem */
		fat_type_t type;

		/** The bootsector */
		fat_bootsector_t fat_bootsector;

		/**
		 * Reads the bootsector (_musst_ be done before accessing the attribute
		 * - init)
		 */
		inline void read_bootsector();

		/**
		 * Reads the clustertable (_musst_ be done before accessing the
		 * attribute - init)
		 */
		inline void read_clustertable();

		/**
		 * Performs I/O on a single cluster.
		 * @param[in] io_type read or write?
		 * @param[in] cluster the cluster to perform I/O on
		 * @param[in] offset the offset to the start of the cluster
		 * @param[in] buffer the buffer to read/write from/to
		 * @param[in] length how mayn bytes should be read/write
		 */
		inline void single_cluster_io(
				fat_io_type_t io_type,
				uint32_t cluster,
				uint32_t offset,
				void* buffer,
				uint32_t length );
};

typedef ::std::shared_ptr< ::iposix::fs::fat_fs > fat_fs_ptr;

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_FS_H_ */
