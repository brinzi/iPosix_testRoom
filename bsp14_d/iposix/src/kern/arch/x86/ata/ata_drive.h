#ifndef _KERN_ARCH_X86_ATA_ATA_DRIVE_H_
#define _KERN_ARCH_X86_ATA_ATA_DRIVE_H_

#include "devices/i_block_device.h"
#include "x86/ata/ata_data_structurs.h"
#include "utils/noncopyable.h"

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


/**
 * Implements an drive
 */
class ata_drive
	: public i_block_device,
	virtual public iposix::utils::Noncopyable
{
	public:
		/**
		 * Contructor creates a hard disk with a specific controller.
		 * @param[in] name name of the drive e.g. hda, hdb, ...
		 * @param[in] controller the relying controller
		 * @param[in] drive_number specifies if this drive is master or slave
		 */
		ata_drive(
				const ::std::string& name,
				ata_drive_number drive_number = ata_none,
				uint16_t base_port = 0,
				uint16_t alt_port = 0 );

		/** Destructor */
		~ata_drive();

		/**
		 * Indentifys the Drive.
		 */
		bool identify();

		// i_block_device

		/**
		 * Reads data from this drive.
		 * @param[in] base_block the base_block on the harddisk to read from
		 * @param[in,out] buffer the buffer where to write the data to
		 * @param[in] length the length of the buffer (in bytes)
		 * @return the lenght of the transfered block (-1 on error)
		 */
		int32_t read_block(
				const uint64_t base_block,
				void* buffer, 
				const uint32_t length );

		/**
		 * Writes data to this drive.
		 * @param[in] base_block the base_block on the harddisk to write to
		 * @param[in,out] buffer the data to read the data from
		 * @param[in] length the length of the buffer
		 * @return the lenght of the transfered block (-1 on error)
		 */
		int32_t write_block(
				const uint64_t base_block,
				const void* buffer, 
				const uint32_t length );

		/**
		 * Returns the size of a block
		 * @return the size
		 */
		uint32_t block_size() const;

		/**
		 * Return the size of the drive in bytes
		 * @retrun the size
		 */
		uint64_t length() const;

		// ~i_block_device

		/** decides which drive is handled by this object (master/slave) */
		const ata_drive_number drive_number;

		/** the information about this drive */
		identify_information information;

		/** the drives base communiction port*/
		const uint16_t base_port;

		/** the drives alternative communcation port */
		const uint16_t alt_port;
};

typedef ::std::shared_ptr< ::iposix::arch::x86::ata::ata_drive > ata_drive_ptr;

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_ATA_ATA_DRIVE_H_ */
