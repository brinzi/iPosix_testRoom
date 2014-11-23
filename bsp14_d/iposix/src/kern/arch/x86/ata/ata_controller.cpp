#include "x86/ata/ata_controller.h"
#include "x86/ata/ata_drive.h"
#include "x86/ata/ata_data_structurs.h"
#include "x86/io_ports/io_ports.h"
#include "logger/logger.h"

//#define __DEBUG_INOUT_DATA__

namespace iposix
{
namespace arch
{
namespace x86
{
namespace ata
{

using ::iposix::arch::x86::io_ports;

void controller_status::dump()
{
	#define dmp( name, value ) \
		logger << message::MORE_VERBOSE << '\t' << #name << ":=\"" \
		<< value << "\"" << endl

	#define dmp_uns( value ) \
		dmp( value, hex << static_cast<uint8_t>( this->status.value) )

	dmp_uns( error );
	dmp_uns( data_request );
	dmp_uns( service );
	dmp_uns( device_fault );
	dmp_uns( ready );
	dmp_uns( busy );

	#undef dmp_uns
	#undef dmp
}

ata_controller::ata_controller()
:	current_drive( ata_none ),
	current_base_port( 0 ),
	current_alt_port( 0 )
{
	this->write_config( controller_config( true, false, false ) );
}

void ata_controller::read_data(void* buffer, uint32_t length)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::read_string16(
				this->current_base_port + RW_DATA_PORT_OFFSET, buffer,
				length / 2);
	}
}

void ata_controller::write_data(void* buffer, uint32_t length)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_string16(
				this->current_base_port + RW_DATA_PORT_OFFSET, buffer , 
				length / 2);
	}
}

uint8_t ata_controller::read_features()
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		return io_ports::read_port8(
				this->current_base_port + R_FEATURES_PORT_OFFSET );
	}
	else
	{
		return 0;
	}
}

error_status ata_controller::read_error()
{
	error_status err;
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		err.bits = io_ports::read_port8(
				this->current_base_port + R_ERROR_PORT_OFFSET );
	}
	return err;
}

hd_selector ata_controller::read_selector()
{
	hd_selector s;
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		s.bits = io_ports::read_port8(
				this->current_base_port + RW_DRIVE_SEL_PORT_OFFSET );
	}
	return s;
}

void ata_controller::write_selector( hd_selector sel )
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_port8(
				this->current_base_port + RW_DRIVE_SEL_PORT_OFFSET, sel.bits );
	}
}

uint8_t ata_controller::read_sector_count()
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		return io_ports::read_port8(
				this->current_base_port + RW_SECTOR_COUNT_PORT_OFFSET );
	}
	else
	{
		return 0;
	}
}

void ata_controller::write_sector_count(uint8_t sec_count)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_port8(
				this->current_base_port + RW_SECTOR_COUNT_PORT_OFFSET,
				sec_count );
	}
}

ata_controller::lba_address ata_controller::read_address()
{
	lba_address address;

	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		address.part[0] = io_ports::read_port8(
				this->current_base_port + RW_ADDRESS1_PORT_OFFSET );
		address.part[1] = io_ports::read_port8(
				this->current_base_port + RW_ADDRESS2_PORT_OFFSET );
		address.part[2] = io_ports::read_port8(
				this->current_base_port + RW_ADDRESS3_PORT_OFFSET );
	}

	return address;
}

void ata_controller::write_address(lba_address address)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_port8(
				this->current_base_port + RW_ADDRESS1_PORT_OFFSET,
				address.part[0] );
		io_ports::write_port8(
				this->current_base_port + RW_ADDRESS2_PORT_OFFSET,
				address.part[1] );
		io_ports::write_port8(
				this->current_base_port + RW_ADDRESS3_PORT_OFFSET,
				address.part[2] );
	}
}

controller_status ata_controller::read_status()
{
	controller_status s;
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		s.bits = io_ports::read_port8(
				this->current_base_port + R_STATUS_PORT_OFFSET );
	}
	return s;
}

void ata_controller::write_command(uint8_t command)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_port8(
				this->current_base_port + W_COMMAND_PORT_OFFSET, command );
	}
}

controller_status ata_controller::read_alt_status()
{
	controller_status s;
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		s.bits = io_ports::read_port8( this->current_alt_port );
	}
	return s;
}

void ata_controller::write_config(const controller_config config)
{
	if ( this->current_base_port != 0 && this->current_alt_port != 0 )
	{
		io_ports::write_port8( this->current_alt_port, config.bits );
	}
}

bool ata_controller::select_drive( const ata_drive& drive )
{
	if (
			( drive.drive_number != current_drive ) ||
			( drive.base_port != current_base_port ) ||
			( drive.alt_port != current_alt_port )
	   )
	{
		if ( drive.drive_number == ata_none )
		{
			logger << message::ERROR << __func__
				<< ": Could not select none "  << endl;

			this->current_base_port = 0;
			this->current_alt_port = 0;

			return false;
		}

		this->current_base_port = drive.base_port;
		this->current_alt_port = drive.alt_port;

		this->write_selector( hd_selector( drive.drive_number == ata_master ) );

		// we need to do this becuase we wait for the hardware to be ready
		io_ports::delay(4); //delay of 400ns
	}

	return true;
}

inline controller_status ata_controller::wait_while_status( controller_status status )
{
	int32_t timeout = WAIT_STATUS_TIMEOUT;
	controller_status cur_status = this->read_alt_status();

	while ( ( cur_status.bits & status.bits ) && ( timeout-- > 0 ) )
	{
		io_ports::delay();
		cur_status = this->read_alt_status();
	}

	if (timeout <= 0)
	{
		logger << message::CRITICAL << __func__
			<< ": Wait for ATA Status reached timeout!" << endl;
		cur_status.dump();
	}

	return this->read_status();
}

int32_t ata_controller::run_command(
		const ata_drive& drive,
		command cmd,
		uint64_t base_sector,
		void* buffer,
		uint32_t sectors)
{
	if (
			( ( sectors >> 8 ) != 0 ) &&
			( !drive.information.has_lba48() )
	   )
	{
		//maximum for LBA28
		sectors &= 0xFF;
	}
	else
	{
		//maximum for LBA48
		sectors &= 0xFFFF;
	}

	// get high and low part of sector count
	uint8_t sec_count_lo = sectors & 0xFF;
	uint8_t sec_count_hi = (sectors >> 8) & 0xFF;

	logger << message::MORE_VERBOSE << __func__
		<< ": base_sector=" << dec << base_sector 
		<< ", sectors=" << dec << sectors << endl;

	lba_address adr[2];

	//get parts of address ( 6 parts )
	adr[0].part[0] = base_sector & 0xFF;
	adr[0].part[1] = ( base_sector >> 8 ) & 0xFF;
	adr[0].part[2] = ( base_sector >> 16 ) & 0xFF;
	adr[1].part[0] = ( base_sector >> 24 ) & 0xFF;
	adr[1].part[1] = ( base_sector >> 32 ) & 0xFF;
	adr[1].part[2] = ( base_sector >> 40 ) & 0xFF;

	bool lba48 = ( ( sec_count_hi != 0 ) || ( (base_sector >> 24) != 0 ) );

	if ( lba48 && !drive.information.has_lba48() )
	{
		//could not address this
		return -1;
	}

	//wait for status
	this->wait_while_status(
			controller_status(false, true, false, false, false, true) );

	//conditional drive selection
	this->select_drive( drive );

	//wait for status
	this->wait_while_status(
			controller_status(false, true, false, false, false, true) );

	//set position and size
	switch ( cmd )
	{
		case write:
		case read:
			if ( lba48 )
			{
				this->write_sector_count( sec_count_hi );
				this->write_address( adr[1] );
			}
			this->write_sector_count( sec_count_lo );
			this->write_address( adr[0] );
			break;

		default:
			break;
	}

	//send command
	switch ( cmd )
	{
		case identify:
				this->write_command( CMD_IDENTIFY_DEVICE );
			break;

		case write:
			this->write_command(
					lba48 ?
					CMD_WRITE_SEC_EXT_RETRY :
					CMD_WRITE_SEC_RETRY
					);
			break;
		
		case read:
			this->write_command(
					lba48 ?
					CMD_READ_SEC_EXT_RETRY :
					CMD_READ_SEC_RETRY
					);
			break;

		default:
			break;
	}

	//wait for status
	switch ( cmd )
	{
		case identify:
		case read:
		case write:
			this->wait_while_status(
					controller_status(false, false, false, false, false, true) );
			break;

		default:
			break;
	}

	//read data
	uint8_t* buf = static_cast<uint8_t*>(buffer);
	for ( uint16_t i = 0; (i < sectors); i++ ) 
	{
		switch ( cmd )
		{
			case write:
				logger << message::MORE_VERBOSE << "writing " << dec
					<< SECTOR_SIZE << " bytes to disk with "
					<< ( sectors - i ) << " sectors remain" << endl;
				this->write_data( &buf[( i << 9 )], SECTOR_SIZE );
				this->write_command( CMD_FLUSH );
				break;

			case identify:
			case read:
				logger << message::MORE_VERBOSE << "reading " << dec
					<< SECTOR_SIZE << " bytes from disk with "
					<< ( sectors - i ) << " sectors remain" << endl;
				this->read_data( &buf[( i << 9 )], SECTOR_SIZE );
				break;

			default:
				break;
		}

		//wait for status
		if ( i < sectors ) //not the last
		{
			this->wait_while_status(
					controller_status(false, false, false, false, false, true) );
		}
		else
		{
			this->wait_while_status(
					controller_status(false, true, false, false, false, true) );
		}
	}

	logger << message::MORE_VERBOSE << __func__ << ": finished" << endl;

#ifdef __DEBUG_INOUT_DATA__
	for (uint16_t i = 0; i < sectors * SECTOR_SIZE; i++)
	{
		logger << message::MORE_VERBOSE << buf[i];
	}
	logger << endl;
#endif //__DEBUG_INOUT_DATA__

	return sectors;
}

bool ata_controller::identify_drive( ata_drive& drive )
{
	if ( sizeof( identify_information ) != 512 )
	{
		logger << message::ERROR << "ERROR: Size of identify is "
			<< sizeof( identify_information ) << endl;

		return false;
	}

	//get informations
	this->run_command( drive, identify, 0, &drive.information.words[0], 1 );

	drive.information.spin();

	return true;
}

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix
