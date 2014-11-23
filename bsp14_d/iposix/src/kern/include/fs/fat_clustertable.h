#ifndef _KERN_INCLUDE_FS_FAT_CLUSTERTABLE_H_
#define _KERN_INCLUDE_FS_FAT_CLUSTERTABLE_H_

#include <stdint.h>

namespace iposix {
namespace fs {

/**
 * Describes the clustertable used by the fat filesystem
 */
class clustertable_t
{
	friend class fat_fs;
	
	public:
		/**
		 * Constructor.
		 * @param[in] type the type of the underlying fat filesystem
		 */
		clustertable_t( const fat_type_t& type );

		/**
		 * Destructor
		 */
		~clustertable_t();

		/**
		 * Sets the size of the cluster table
		 * @param[in] size the new size
		 */
		void size(uint32_t size);

		/**
		 * Returns the size of the clustertable
		 * @return the size of the clustertable
		 */
		uint32_t size() const;

		void norm_node_add( uint32_t add );

		/**
		 * Returns the successor of a cluster
		 * @param[in] cluster the cluster number of intresst
		 * @return the cluster which is the successor
		 */
		uint32_t successor( uint32_t cluster ) const;

		uint32_t chain_length( uint32_t cluster ) const;

		/**
		 * Returns if the cluster has a successor
		 * @param[in] cluster the cluster number of intresst
		 * @return if the successor exists
		 */
		bool has_successor( uint32_t cluster ) const;

		/**
		 * Returns if the cluster in the clustertable is free
		 * @param[in] cluster the cluster number which should be tested
		 * @return true if the cluster is free
		 */
		bool is_free( uint32_t cluster ) const;

		/**
		 * Returns if the cluster in the clustertable is defect.
		 * @param[in] cluster the cluster number which should be tested
		 * @return true if the cluster is defect
		 */
		bool is_defect( uint32_t cluster ) const;

		/**
		 * Returns if the cluster in the clustertable is the end of a file/dir
		 * @param[in] cluster the cluster number which should be tested
		 * @return true if the cluster is the end of a file/dir
		 */
		bool is_end( uint32_t cluster ) const;

	protected:
		static const uint16_t FAT12_FREE	= 0x0;
		static const uint16_t FAT12_DEFECT	= 0xFF7;
		static const uint16_t FAT12_END		= 0xFF8;

		static const uint16_t FAT16_FREE	= 0x0;
		static const uint16_t FAT16_DEFECT	= 0xFFF7;
		static const uint16_t FAT16_END		= 0xFFF8;

		static const uint32_t FAT32_FREE	= 0x0;
		static const uint32_t FAT32_DEFECT	= 0x0FFFFFF7;
		static const uint32_t FAT32_END		= 0x0FFFFFF8;

		static const uint32_t BYTES = 12;
		static const uint32_t FAT12_ENTRYS = 8;
		static const uint32_t FAT16_ENTRYS = 6;
		static const uint32_t FAT32_ENTRYS = 3;

	private:
		/** dirty struct for fat12 entrys */
		struct __attribute__((packed)) cl_fat12_entry_t
		{
			unsigned value0:12;
			unsigned value1:12;
			unsigned value2:12;
			unsigned value3:12;
			unsigned value4:12;
			unsigned value5:12;
			unsigned value6:12;
			unsigned value7:12;
		};

		/**
		 * A Union which hold the different types of fat clusterentrys
		 */
		union __attribute__((packed)) clustertable_data_t
		{
			uint8_t bytes[BYTES];
			cl_fat12_entry_t fat12_entry;
			uint16_t fat16_entry[FAT16_ENTRYS];
			uint32_t fat32_entry[FAT32_ENTRYS];
		};

		/** The data hold by this clustertable */
		clustertable_data_t* data;

		/** The size */
		uint32_t size_bytes;

		/** stupid */
		uint32_t add;

		/** The type */
		const fat_type_t type;

		/**
		 * Return the value in the clustertable on a specific position
		 * @param[in] cluster the postition
		 * @return the value
		 */
		uint32_t value(uint32_t cluster) const;

		/**
		 * Dumps out some Information about the clustertable.
		 */
		void dump() const;
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_CLUSTERTABLE_H_ */
