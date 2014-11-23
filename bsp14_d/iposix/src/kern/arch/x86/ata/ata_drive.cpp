#include "x86/ata/ata_drive.h"
#include "x86/ata/ata_controller.h"
#include "logger/logger.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{
namespace ata
{

ata_drive::ata_drive(
		const ::std::string& name,
		ata_drive_number drive_number,
		uint16_t base_port,
		uint16_t alt_port)
	:	i_block_device( name ),
	drive_number( drive_number ),
	information(),
	base_port( base_port ),
	alt_port( alt_port )
{ }

ata_drive::~ata_drive()
{ }

inline ata_controller& controller()
{
	return ::iposix::utils::Singleton< ata_controller >::instance();
}

int32_t ata_drive::read_block(uint64_t base_block, void* buffer, uint32_t length)
{
	if ( this->drive_number == ata_none ) //is this a drive
	{
		logger << message::ERROR << "ata_drive " << __func__
			<< ": Try to execute Command but drive is whether master nor slave."
			<< endl;

		return -1;
	}
	else
	{
		//run a read command
		return controller().run_command(
				*this,
				ata_controller::read,
				base_block,
				buffer,
				length );
	}
}

int32_t ata_drive::write_block(uint64_t base_block, const void* buffer, uint32_t length)
{
	if ( this->drive_number == ata_none ) // is this a drive
	{
		logger << message::ERROR << "ata_drive " << __func__
			<< ": Try to execute Command but drive is whether master nor slave."
			<< endl;

		return -1;
	}
	else
	{
		//run a write command
		return controller().run_command(
				*this,
				ata_controller::write,
				base_block,
				const_cast<void*>(buffer), //hm we now that this method will not write to the buffer
				length );
	}
}

bool ata_drive::identify()
{
	if ( this->drive_number == ata_none ) // is this a drive
	{
		logger << message::ERROR << "ata_drive " << __func__
			<< ": Try to execute Command but drive is whether master nor slave." 
			<< endl;

		return false;
	}
	else
	{
		//start identifying
		controller().identify_drive( *this );

		//dump the information
		this->information.dump();
	}

	return true;
}

uint32_t ata_drive::block_size() const
{
	return ata_controller::SECTOR_SIZE;
}

uint64_t ata_drive::length() const
{
	return this->information.i.number_of_acc_sector_lba48 * this->block_size();
}

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix
