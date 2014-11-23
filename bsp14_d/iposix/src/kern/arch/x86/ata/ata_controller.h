#ifndef _KERN_ARCH_X86_ATA_ATA_CONTROLLER_H_
#define _KERN_ARCH_X86_ATA_ATA_CONTROLLER_H_

#include "x86/ata/ata_data_structurs.h"

#include "utils/singleton.hpp"

#include <stdint.h>
#include <memory>

namespace iposix
{
namespace arch
{
namespace x86
{
namespace ata
{

/* Predefine */
//union identify_information;
class ata_drive;

/**
 * Implements an ata controller.
 * \todo TODO currently only lba 24(not 28 is implemented
 * \todo TODO currentyl lba 28 is deactivated (see "lba48 = true;")
 * \todo TODO no interrupt handling is implemented
 * \todo TODO Drive errors are ignored
 * \todo TODO DMA is not implemented
 * \todo TODO no synchronisation is implemented
 */
class ata_controller
	: public ::iposix::utils::Singleton< ::iposix::arch::x86::ata::ata_controller >
{
	friend class ::iposix::utils::Singleton< ata_controller >;

	public:
		/**
		 * The Command which could be executed.
		 */
		enum command
		{
			identify,
			write,
			read
		};

		/**
		 * Executes a command on a specific drive
		 * @param[in] drive the drive to run the command on
		 * @param[in] cmd the command to run
		 * @param[in] base_sector the command specific base_sector on the drive
		 * @param[in] buffer the data buffer for the command (if needed)
		 * @param[in] sectors the number of sectors to transfer
		 * @return the number of transfered sectors ( -1 on errors )
		 */
		int32_t run_command(
				const ata_drive& drive,
				command cmd,
				uint64_t base_sector,
				void* buffer,
				uint32_t sectors);

		bool identify_drive(
				ata_drive& drive
				);

		/** The Size of a HardDisk Sector (in Bytes) */
		static const uint32_t SECTOR_SIZE				= 512;
	
	private:
		/**
		 * Constructor for an ata controller
		 * @param[in] base_port the base communication port
		 * @param[in] alt_port the alternative communication port
		 */
		ata_controller();

		/** Destructor */
		~ata_controller()
		{ };

	private:
		/** Describes a lba address */
		struct lba_address
		{
			uint8_t part[3];
		};

		/** Reads Data from HD */
		inline void read_data(void* buffer, uint32_t length);
		/** Write Data to HD */
		inline void write_data(void* buffer, uint32_t length);

		/** Reads the HD Features */
		inline uint8_t read_features();
		/** Read the Error Informations */
		inline error_status read_error();

		/** Reads the Disk Selector */
		inline hd_selector read_selector();
		/** Write the Disk Selector */
		inline void write_selector(hd_selector sel);

		/** Reads the Sector Count */
		inline uint8_t read_sector_count();
		/** Writes the Sector Count */
		inline void write_sector_count(uint8_t sec_count);

		/** Reads a lba address */
		inline lba_address read_address();
		/** Write a lba address */
		inline void write_address(lba_address address);

		/** Reads the controllers status */
		inline controller_status read_status();
		/** Write a controller command */
		inline void write_command(uint8_t command);
		
		/** Read the controllers alternativ status */
		inline controller_status read_alt_status();
		/** Write the controllers config */
		inline void write_config(controller_config config);

		/** Selects a drive */
		inline bool select_drive( const ata_drive& drive );

		/** Wait for a specific status to occur */
		inline controller_status wait_while_status(controller_status status);

		/** the last/current used drive */
		ata_drive_number current_drive;

		/** the last/current used base communiction port*/
		uint16_t current_base_port;

		/** the last/current used alternative communcation port */
		uint16_t current_alt_port;
};

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_ATA_ATA_CONTROLLER_H_ */
