#ifndef _KERN_INCLUDE_FS_FAT_DIRECTORY_ENTRY_HPP_
#define _KERN_INCLUDE_FS_FAT_DIRECTORY_ENTRY_HPP_

#include "fs/fat_type.h"
#include <time.h>
#include <stdint.h>

namespace iposix {
namespace fs {


/**
 * Describes the fat time type
 */
struct __attribute__((packed)) fat_time_t
{
	public:
		unsigned hour:5;
		unsigned minute:6;
	private:
		unsigned second_div2:5;
	public:
		/** Constructor */
		fat_time_t()
			: hour( 0 ),
			minute( 0 ),
			second_div2( 0 )
		{ }

		/**
		 * Sets and converts the seconds
		 * @param[in] second the seconds
		 */
		void second( const uint8_t second )
		{
			this->second_div2 = (second >> 1);
		}

		/**
		 * Returns the converted seconds
		 * @return the converted seconds
		 */
		uint8_t second() const
		{
			return (this->second_div2 << 1);
		}

		/**
		 * Returns the converted unix time
		 * \todo this method does not handle leap-time or crazy stuff like that
		 * @return the converted unix time
		 */
		time_t unix_time() const
		{
			time_t time( hour );

			time *= 60;
			time += minute;
			time *= 60;
			time += this->second();

			return time;
		}
};

/**
 * Describes the fat date type
 */
struct __attribute__((packed)) fat_date_t
{
	public:
		/** The year after 1980 */
		unsigned year:7;
		/** The month 1..12 */
		unsigned month:4;
		/** The day 1..31 */
		unsigned day:5;

		/** Constructor */
		fat_date_t()
			: year( 0 ),
			month( 0 ),
			day( 0 )
		{ }

		/**
		 * Returns the converted unix time
		 * \todo this method does not handle leap-time or crazy stuff like that
		 * @return the converted unix time
		 */
		time_t unix_time() const
		{
			// TODO This is wrong but works for moment
			time_t time( year * 365 ); //em yeah would be nice

			time += ( this->month - 1 ) * 31 ; //em yeah that's a bit wrong
			time += ( this->day - 1 );

			return time;
		}
};

/**
 * Describes the fat directory entry type
 */
struct __attribute__((packed)) fat_directory_entry_t
{
	public:
		/** the base name */
		char name[8];										//0..7
		/** the extension */
		char extension[3];									//8..10
		/** the access attributes */
		struct __attribute__((packed)) attr_t				//11
		{
			public:
				unsigned write_protected:1;							//0
				unsigned hidden:1;									//1
				unsigned system:1;									//2
				unsigned volume_label:1;							//3
				unsigned directory:1;								//4
				unsigned archive:1;									//5
			private:
				unsigned unused:2;									//6..7
			public:
				attr_t()
					: write_protected( 0 ),
					hidden( 0 ),
					system( 0 ),
					volume_label( 0 ),
					directory( 0 ),
					archive( 0 ),
					unused( 0 )
				{ }
		} attributes;
	private:
		uint8_t reserved;									//12
	public:
		uint8_t creation_time_millis_div10;					//13
		fat_time_t create_time;								//14..15
		fat_date_t create_date;								//16..17
		fat_date_t last_access_date;						//18..19
		union __attribute__((packed))						//20..21
		{
			uint16_t fat12_EA_index;
			uint16_t fat16_EA_index;
			uint16_t fat32_cluster_high;
		};
		fat_time_t last_modification_time;					//22..23
		fat_date_t last_modification_date;					//24..25
		union __attribute__((packed))						//26..27
		{
			uint16_t fat12_cluster;
			uint16_t fat16_cluster;
			uint16_t fat32_cluster_low;
		};
		/** Size in Bytes */
		uint32_t size;										//28..31

		/** Constructor */
		fat_directory_entry_t()
			: name(),
			extension(),
			attributes(),
			reserved( 0 ),
			creation_time_millis_div10( 0 ),
			create_time(),
			create_date(),
			last_access_date(),
			fat32_cluster_high( 0 ),
			last_modification_time(),
			last_modification_date(),
			fat32_cluster_low( 0 ),
			size(0)
		{ }

		/**
		 * Returns the cluster for this file. This abstracts from different
		 * handling between filesystem type.
		 * @param[in] type the fat type of the underlying filesystem
		 * @return the cluster number
		 */
		uint32_t cluster( const fat_type_t type) const;

		/** dumps the information */
		void dump() const;

		/**
		 * Returns the converted filename
		 */
		const ::std::string filename() const;

		/**
		 * Returns the converted unix time of creation
		 */
		time_t create_time_unix() const
		{
			time_t time( this->create_time.unix_time() );
			time += this->create_date.unix_time();
			return time;
		}

		/**
		 * Returns the converted unix time of last access
		 */
		time_t access_time_unix() const
		{
			time_t time( this->last_access_date.unix_time() );
			return time;
		}

		/**
		 * Returns the converted unix time of last modification
		 */
		time_t modify_time_unix() const
		{
			time_t time( this->last_modification_time.unix_time() );
			time += this->last_modification_date.unix_time();
			return time;
		}
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_DIRECTORY_ENTRY_HPP_ */
