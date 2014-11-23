#ifndef _KERN_INCLUDE_FS_PARTITIONTABLE_CRAWLER_H_
#define _KERN_INCLUDE_FS_PARTITIONTABLE_CRAWLER_H_

#include "utils/singleton.hpp"

#include <stdint.h>
#include <memory>
#include <string>

namespace iposix
{

/* Predefine */
namespace arch {
class i_block_device;
}

/**
 * Contains a partitiontable crawler.
 */
namespace fs
{

/* Predefine */
class pc_partitiontable_entry;

/**
 * Describes a partitiontable crawler which analyses an abstract drive and adds
 * all found partitions as device nodes to the device filesystem.
 */
class partitiontable_crawler
	: virtual public ::iposix::utils::Singleton< partitiontable_crawler >
{
	friend class ::iposix::utils::Singleton< partitiontable_crawler >;

	private:
		/** A shared pointer to an abstract drive */
		typedef ::std::shared_ptr< ::iposix::arch::i_block_device >
			i_block_device_ptr;

	public:
		/**
		 * Adds all filesystems hold by this drive
		 * @param[in] drive the filesystem node which should be probed
		 */
		void probe_filesystems( i_block_device_ptr drive );

	private:

		/**
		 * Adds a filesystem node to dev_fs
		 * @param[in] drive the filesystem node which should be probed
		 * @param[in] name the name of the new node
		 * @param[in] first_address of the belonging drive accessed by this FS
		 * @param[in] last_address of the belonging drive accessed by this FS
		 */
		inline void add_filesystem( 
				i_block_device_ptr drive,
				::std::string name, 
				uint64_t first_address,
				uint64_t last_address );

		/**
		 * Adds a filesystem node to dev_fs
		 * @param[in] node virtual node which abstracts the device
		 * @param[in] drive the filesystem node which should be probed
		 * @param[in,out] counter a counter used for counting the found
		 * filesystems
		 * @param[in] part_entry the pc_partitiontable_entry which describes the filesystem
		 * @param[in] addition_to_sector the offset which should be added to
		 * every occurence of sectors in the entry
		 */
		inline void add_filesystem(
				i_block_device_ptr node,
				uint32_t& counter,
				const pc_partitiontable_entry& part_entry,
				const uint64_t addition_to_sector = 0
				);

		/**
		 * Tests if this entry describes a extented partition_entry.
		 * @param[in] part_entry the entry which should be tested
		 * @return true if this entry describes a extended partition_entry
		 */
		inline bool is_extended(
				const pc_partitiontable_entry& part_entry
				);

		/**
		 * Returns the partition table in the given sector.
		 * @param[in] drive the filesystem node which should be probed
		 * @param[in] buffer the buffer where to write the partitiontable to
		 * (has to be sizeof(pc_partitiontable_entry) * 4 long)
		 * @param[in] sector the sector which should be read
		 */
		inline void get_table( 
				i_block_device_ptr drive,
				pc_partitiontable_entry* buffer, 
				const uint64_t sector
				);

		/**
		 * Does a recursive probing of extended partitions beginning with this
		 * entry.
		 * @param[in] drive the filesystem node which should be probed
		 * @param[in,out] counter a counter used for counting the found
		 * filesystems
		 * @param[in] part_entry the entry which should be probed
		 * @param[in] relative_to_sector the sector this entry should be interpreted relative to
		 */
		inline void probe_extfilesystems(
				i_block_device_ptr drive,
				uint32_t& counter,
				const pc_partitiontable_entry& part_entry,
				const uint64_t relative_to_sector = 0
				);

};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_PARTITIONTABLE_CRAWLER_H_ */
