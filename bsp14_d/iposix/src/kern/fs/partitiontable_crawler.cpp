#include "fs/partitiontable_crawler.h"
#include "fs/pc_partitiontable.h"
#include "fs/dev_fs.h"
#include "fs/dev_fs_node.h"
#include "devices/i_block_device.h"
#include "logger/logger.h"

#include <cstring>
#include <string>
#include <sstream>

namespace iposix {
namespace fs {

inline void partitiontable_crawler::add_filesystem( 
		i_block_device_ptr drive,
		::std::string name, 
		uint64_t first_address,
		uint64_t last_address )
{
		logger << message::MORE_VERBOSE << __func__
			<< ": creating device_node " << name.c_str() << endl;

		dev_fs_node_ptr node( 
				dev_fs_node_ptr(
					new dev_fs_node( name, drive, first_address, last_address )
					)
				);

		::iposix::utils::Singleton< dev_fs >::instance().root_node->add_subnode( node );
}

inline void partitiontable_crawler::add_filesystem( 
		i_block_device_ptr drive,
		uint32_t& counter,
		const pc_partitiontable_entry& part_entry,
		const uint64_t addition_to_sector
		)
{
	logger << message::MORE_VERBOSE << hex << part_entry.bootable.bits << '\t'
		<< hex << part_entry.first_chs[0]
		<< hex << part_entry.first_chs[1]
		<< hex << part_entry.first_chs[2] << '\t'
		<< hex << part_entry.partition_type_bits << '\t'
		<< hex << part_entry.last_chs[0]
		<< hex << part_entry.last_chs[1]
		<< hex << part_entry.last_chs[2] << '\t'
		<< dec << part_entry.first_sector + addition_to_sector << '\t'
		<< dec << part_entry.sector_count << endl;
	
	if ( part_entry.type() != partition_type::p_unused )
	{
		uint32_t sector_size = drive->block_size();
		uint64_t first_address = ( part_entry.first_sector + addition_to_sector ) * sector_size;
		uint64_t last_address =
			( 
			 part_entry.first_sector + addition_to_sector + part_entry.sector_count
			) * sector_size;


		::std::string name;

		//append the counter to the device name
		char buffer[255]; //just have a buffer here
		::std::stringstream ss( buffer );
		ss << drive->name;
		ss << counter++;
		ss >> name ;

		this->add_filesystem( drive, name, first_address, last_address );

	}
}

inline bool partitiontable_crawler::is_extended(
		const pc_partitiontable_entry& part_entry
		)
{
	return ( 
			( part_entry.type() == partition_type::p_ext_part ) || 
			( part_entry.type() == partition_type::p_ext_part_bios )
		   );
}

inline void partitiontable_crawler::get_table( 
		i_block_device_ptr drive,
		pc_partitiontable_entry* buffer, 
		const uint64_t sector )
{
	char buf[pc_partitiontable_entry::SECTOR_SIZE];

	//read the table
	drive->read( sector, &buf[0], pc_partitiontable_entry::SECTOR_SIZE );

	//copy the wanted content to the buffer
	::std::memcpy( 
			&buffer[0], 
			&buf[pc_partitiontable_entry::TABLE_OFFSET_TO_SECTOR], 
			4 * sizeof(pc_partitiontable_entry)
		  );
}

void partitiontable_crawler::probe_extfilesystems( 
		i_block_device_ptr drive,
		uint32_t& counter,
		const pc_partitiontable_entry& part_entry, 
		const uint64_t relative_to_sector 
		)
{
	pc_partitiontable_entry entrys[4];

	//get the table
	this->get_table( drive, &entrys[0], part_entry.first_sector + relative_to_sector );

	//all filesystem in a extended partitiontable are relative to the
	//partitiontable it is hold by
	this->add_filesystem( drive, counter, entrys[0], part_entry.first_sector);

	if ( is_extended( entrys[1] ) )
	{
		uint64_t new_relative_to_sector = relative_to_sector;

		if ( new_relative_to_sector == 0 )
		{
			//all extended partition tables are relative to the first extended
			//partitiontable except the first table it self
			new_relative_to_sector = part_entry.first_sector;
		}

		this->probe_extfilesystems( drive, counter, entrys[1], new_relative_to_sector );
	}
}

void partitiontable_crawler::probe_filesystems( i_block_device_ptr drive )
{
	uint32_t counter( 1 );
	pc_partitiontable_entry entrys[4];

	this->add_filesystem( drive, drive->name, uint64_t(0), drive->length() );

	//get the first table
	this->get_table( drive, &entrys[0], 0 );

	//there are four entrys in the first table so we probe them all
	for (uint32_t i = 0; i < 4; i++)
	{
		if ( is_extended( entrys[i] ) )
		{
			this->probe_extfilesystems( drive, counter, entrys[i] );
		}
		else
		{
			//filesystem in the first partitiontable are relative to 0 (the
			//first_partition_table)
			this->add_filesystem( drive, counter, entrys[i]);
		}
	}
}

} //namespace fs
} //namespace iposix
