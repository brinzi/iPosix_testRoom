#include "x86/ata/ata.h"
#include "x86/ata/ata_drive.h"
#include "x86/multiboot/multiboot.h"
#include "fs/partitiontable_crawler.h"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{
namespace ata
{

ata::ata()
	:	controllers()
{ }

ata::~ata() 
{ }

void ata::init()
{
	logger << message::MORE_VERBOSE << "----------ata::" << __func__ << "-----------" << endl;
	const multiboot& mb = ::iposix::utils::Singleton< multiboot >::instance();

	logger << message::MORE_VERBOSE << "Initializing ATA Harddisks." << endl;

	// does multiboot reports the drive information
	if ( ! mb.drives_field_present() )
	{
		logger << message::ERROR << "Error: No drive field in Multiboot header" << endl
			<< message::MORE_VERBOSE << "---------~ata::" << __func__ << "-----------" << endl;
		return;
	}

	uint32_t length = mb.get_drive_map_length();
	if ( length == 0 )
	{
		logger << message::WARN << "Warning: No drives" << endl
			<< message::MORE_VERBOSE << "---------~ata::" << __func__ << "-----------" << endl;
		return;
	}

	multiboot::drive_map* drive_ptr = mb.get_drive_map();
	multiboot::drive_map* drive_ptr_end =
		reinterpret_cast<multiboot::drive_map*>(
				reinterpret_cast<uint8_t*>(
					mb.get_drive_map())
				+ length);

	logger << message::MORE_VERBOSE << "Map Length" << length << endl;

	// iterate throught the drive map
	for (;drive_ptr < drive_ptr_end;
			drive_ptr =
			reinterpret_cast<multiboot::drive_map*>(
				reinterpret_cast<uint8_t*>( drive_ptr ) +
				drive_ptr->size )
		)
	{
		logger << message::MORE_VERBOSE << "Drive Number:" << hex 
			<< drive_ptr->drive_number << dec
			<< " Mode:" << drive_ptr->drive_mode;

		if (drive_ptr->drive_mode != 1)
		{
			logger << endl << message::ERROR << "Only LBA Harddisks implemented" << endl;
			continue;
		}

		logger << message::MORE_VERBOSE << " Cylinders:" << drive_ptr->drive_cylinders 
			<< " Heads:" << drive_ptr->drive_heads 
			<< " Sectors:" << drive_ptr->drive_sectors << endl;

		uint16_t* port = &drive_ptr->drive_ports;
		if (*port == 0) //get the ports. it's null terminated so we check the first entry is 0
		{
			logger << message::WARN << "Warning: No Drive Ports. "
				<< message::MORE_VERBOSE << "Using standard Ports." 
				<< message::WARN << endl;

			ata_drive_ptr new_drive = ata_drive_ptr();

			//initilize the drives
			switch ( drive_ptr->drive_number )
			{
				case 0x80:
					new_drive = ata_drive_ptr(
							new ata_drive(
								"hda",
								ata_master,
								PRIMARY_BUS_BASE_PORT,
								PRIMARY_BUS_ALT_PORT
								)
							);
					break;

				case 0x81:
					new_drive = ata_drive_ptr(
							new ata_drive(
								"hdb",
								ata_slave,
								PRIMARY_BUS_BASE_PORT,
								PRIMARY_BUS_ALT_PORT
								)
							);
					break;

				case 0x82:
					new_drive = ata_drive_ptr(
							new ata_drive(
								"hdc",
								ata_master,
								SECONDARY_BUS_BASE_PORT,
								SECONDARY_BUS_ALT_PORT
								)
							);
					break;

				case 0x83:
					new_drive = ata_drive_ptr(
							new ata_drive(
								"hdd",
								ata_slave,
								SECONDARY_BUS_BASE_PORT,
								SECONDARY_BUS_ALT_PORT
								)
							);
					break;

				default:
					logger << message::WARN << "Drive Number " << hex
						<< drive_ptr->drive_number << " unhandled." << endl;
					continue;
			}

			if ( new_drive ) //if the pointer is valid
			{
				//add the drive to the drives vector
				this->drives.push_back( new_drive );
				new_drive->identify();
				::iposix::utils::Singleton< 
					::iposix::fs::partitiontable_crawler
					>::instance().probe_filesystems( new_drive );
			}
		}
		else
		{
			for ( ; *port; port++ )
			{
				logger << message::MORE_VERBOSE << "Drive Ports: " << *port << endl;
			}

			logger << message::ERROR << "Multiboot with Ports not implemented!" << endl;
		}
	}

	logger << message::MORE_VERBOSE << "---------~ata::" << __func__ << "-----------" << endl;
}

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix
