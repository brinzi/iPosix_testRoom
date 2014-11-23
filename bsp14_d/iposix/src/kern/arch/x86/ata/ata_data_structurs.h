#ifndef _KERN_ARCH_X86_ATA_ATA_DATA_STRUCTURS_H_
#define _KERN_ARCH_X86_ATA_ATA_DATA_STRUCTURS_H_

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

/** base_port offset to read or write data from or to */
static const uint16_t RW_DATA_PORT_OFFSET			= 0x0;
/** base_port offset to read drive Features from */
static const uint16_t R_FEATURES_PORT_OFFSET		= 0x1;
/** base_port offset to read drive error information from (mostly used by ATAPI) */
static const uint16_t R_ERROR_PORT_OFFSET			= 0x1;
/** base_port offset to write sector count to */
static const uint16_t RW_SECTOR_COUNT_PORT_OFFSET	= 0x2;
/** base_port offset to write first/fourth part of the address to */
static const uint16_t RW_ADDRESS1_PORT_OFFSET		= 0x3;
/** base_port offset to write second/fiveth part of the address to */
static const uint16_t RW_ADDRESS2_PORT_OFFSET		= 0x4;
/** base_port offset to write third/sixth part of the address to */
static const uint16_t RW_ADDRESS3_PORT_OFFSET		= 0x5;
/** base_port offset to read or write drive selector from or to */
static const uint16_t RW_DRIVE_SEL_PORT_OFFSET		= 0x6;
/** base_port offset to write command to */
static const uint16_t W_COMMAND_PORT_OFFSET			= 0x7;
/** base_port offset to read status from - this musst be done to reset the interrupt */
static const uint16_t R_STATUS_PORT_OFFSET			= 0x7;

/** Alt_Port offset to write controller config to */
static const uint16_t W_DEVICE_CTRL_PORT_OFFSET		= 0x0;
/** alt_port offset to read alternativ status from - this does not reset the interrupt */
static const uint16_t R_ALT_STATUS_PORT_OFFSET		= 0x0;


/** Controller Command to identify a device */
static const uint8_t CMD_IDENTIFY_DEVICE		= 0xEC;
/** Controller Command to format a track */
static const uint8_t CMD_FORMAT					= 0x50;

/** Controller Command to read a sector and retry on errors in lba 28 mode */
static const uint8_t CMD_READ_SEC_RETRY			= 0x20;
/** Controller Command to read a sector and retry on errors in lba 48 mode */
static const uint8_t CMD_READ_SEC_EXT_RETRY		= 0x24;

/** Controller Command to write a sector and retry on errors in lba 28 mode */
static const uint8_t CMD_WRITE_SEC_RETRY		= 0x30;
/** Controller Command to write a sector and retry on errors in lba 48 mode */
static const uint8_t CMD_WRITE_SEC_EXT_RETRY	= 0x34;

/** Controller Command to flush the pipeline after a read/write command in lba28 mode */
static const uint8_t CMD_FLUSH					= 0xE7;
/** Controller Command to flush the pipeline after a read/write command in lba48 mode */
static const uint8_t CMD_FLUSH_EXT				= 0xEA;

/** read sectors without retry */
static const uint8_t CMD_READ_SEC				= 0x21;
/** read long with retry */
static const uint8_t CMD_READ_LONG_RETRY		= 0x22;
/** read long without retry */
static const uint8_t CMD_READ_LONG			= 0x23;
/** write sectors without retry */
static const uint8_t CMD_WRITE_SEC			= 0x31;
/** write long with retry */
static const uint8_t CMD_WRITE_LONG_RETRY		= 0x32;
/** write long without retry */
static const uint8_t CMD_WRITE_LONG			= 0x33;

/** The Timeout which decides how long to wait for a status. */
static const int32_t WAIT_STATUS_TIMEOUT		= 100000;

/** The specification code for ATAPI devices */
static const uint16_t ATAPI_SPEC_CONFIGURATION = 0x14eb;

/** The specification code for ATA devices */
static const uint16_t ATA_SPEC_CONFIGURATION = 0x0;

/**
 * Describes the identify command informations.
 * have a look at http://www.bustrace.com/bustrace6/generate.htm
 * \todo TODO incomplet
 */
union identify_information
{
	uint16_t words[256];
	struct __attribute__ ((packed))
	{
		public:
			struct __attribute__ ((packed))					//0
			{
				public:
					unsigned command_packet:2;					//0..1
					unsigned response_incomplete:1;				//2
				private:
					unsigned unknown1:2;						//3..4
				public:
					unsigned drive_request:2;					//5..6
					unsigned removeable_media:1;				//7
					unsigned device_type:5;						//8..12
				private:
					unsigned unknown2:1;						//13
				public:
					unsigned atapi_device:2;					//14..15
			} word0;
			uint16_t num_of_log_cyl;						//1
			uint16_t spec_configuration;					//2
			uint16_t num_of_log_heads;						//3
		private: 
			uint16_t unknown1[2];							//4..5
		public:
			uint16_t num_of_secs_per_head;					//6
		private:
			uint16_t unknown2[3];							//7..9
		public:
			char serial_number[20];							//10..19
		private:
			uint16_t unknown3[3];							//20..22
		public:
			char firmware_revision[8];						//23..26
			char model_number[40];							//27..46
			uint16_t num_of_sec_transfered_by_interrupt;	//47
			uint16_t trusted_computing_features;			//48
		private:
			uint16_t unknown4[4];							//49..52
		public:
			struct __attribute__ ((packed))					//53
			{
				private:
					unsigned unknown1:1;						//0
				public:
					unsigned field_88:1;						//1
					unsigned fields_64_70:1;					//2
				private:
					unsigned unknown2:13;						//3..15
			} valid;
		private:
			uint16_t unknown5[5];							//54..58
		public:
			uint16_t multible_sector_setting_valid;			//59
			uint32_t number_of_acc_sector_lba28;			//60..61
		private:
			uint16_t unknown6;								//62
		public:
			struct __attribute__ ((packed))					//63
			{
				public:
					unsigned mode0_supported:1;					//0
					unsigned mode1_supported:1;					//1
					unsigned mode2_supported:1;					//2
				private:
					unsigned unknown1:5;						//3..7
				public:
					unsigned mode0_selected:1;					//8
					unsigned mode1_selected:1;					//9
					unsigned mode2_selected:1;					//10
				private:
					unsigned unknown2:5;						//11.15
			} multiword_dma_settings;
			uint16_t pio_modes_supported;					//64 //TODO incomplet
			uint16_t min_multiword_cycle_time_in_nsec;		//65
			uint16_t rec_multiword_cycle_time_in_nsec;		//66
			uint16_t min_pio_cycle_time_in_nsec;			//67
			uint16_t min_pio_IORDY_cycle_time_in_nsec;		//68
		private:
			uint16_t unknown7[13];							//69..81
		public:
			struct __attribute__ ((packed))					//82
			{
				public:
					unsigned smart_feature:1;					//0
					unsigned security_mode_feature:1;			//1
					unsigned removeable_media_feature:1;		//2
					unsigned power_management_feature:1;		//3
				private:
					unsigned unknown1:1;						//4
				public:
					unsigned write_cache:1;						//5
					unsigned look_ahead:1;						//6
					unsigned release_interrupt:1;				//7
					unsigned service_interrupt:1;				//8
					unsigned reset_command:1;					//9
					unsigned host_protected_area:1;				//10
				private:
					unsigned unknown2:1;						//11
				public:
					unsigned write_buffer:1;					//12
					unsigned read_buffer:1;						//13
					unsigned nop_command:1;						//14
				private:
					unsigned unknown3:1;						//15
				public:										//83
					unsigned download_microcode:1;				//0
					unsigned rw_dma_queued:1;					//1
					unsigned cfa_feature:1;						//2
					unsigned advanced_power_management:1;		//3
					unsigned removeable_media_notification:1;	//4
					unsigned standby_power_up:1;				//5
					unsigned spinup_after_powerup:1;			//6
				private:
					unsigned unknown4:1;						//7
				public:
					unsigned security_extension:1;				//8
					unsigned auto_acoustic_management:1;		//9
					unsigned addresses_48bit:1;					//10
					unsigned config_overlay:1;					//11
					unsigned flush_cache_command:1;				//12
					unsigned flush_cache_extension:1;			//13
				private:
					unsigned unknown5:2;						//14..15
				public:										//84
					unsigned smart_error_log:1;					//0
					unsigned smart_selftest:1;					//1
					unsigned media_serial_number:1;				//2
					unsigned media_card_pass_through:1;			//3
					unsigned streaming_feature:1;				//4
					unsigned general_purpose_log:1;				//5
					unsigned dma_fua_extension:1;				//6
					unsigned dma_queued_fua_extension:1;		//7
					unsigned world_wide_name:1;					//8
					unsigned urg_read_stream:1;					//9
					unsigned urg_write_stream:1;				//10
					unsigned timelimit_feature:1;				//11
					unsigned continuous_timelimit_feature:1;	//12
				private:
					unsigned unknown:3;							//13..15
			} supported_features;
			struct __attribute__ ((packed))					//85
			{
				public:
					unsigned smart_feature:1;					//0
					unsigned security_mode_feature:1;			//1
					unsigned removeable_media_feature:1;		//2
					unsigned power_management_feature:1;		//3
				private:
					unsigned unknown1:1;						//4
				public:
					unsigned write_cache:1;						//5
					unsigned look_ahead:1;						//6
					unsigned release_interrupt:1;				//7
					unsigned service_interrupt:1;				//8
					unsigned reset_command:1;					//9
					unsigned host_protected_area:1;				//10
				private:
					unsigned unknown2:1;						//11
				public:
					unsigned write_buffer:1;					//12
					unsigned read_buffer:1;						//13
					unsigned nop_command:1;						//14
				private:
					unsigned unknown3:1;						//15
				public:										//86
					unsigned download_microcode:1;				//0
					unsigned rw_dma_queued:1;					//1
					unsigned cfa_feature:1;						//2
					unsigned advanced_power_management:1;		//3
					unsigned removeable_media_notification:1;	//4
					unsigned standby_power_up:1;				//5
					unsigned spinup_after_powerup:1;			//6
				private:
					unsigned unknown4:1;						//7
				public:
					unsigned security_extension:1;				//8
					unsigned auto_acoustic_management:1;		//9
					unsigned addresses_48bit:1;					//10
					unsigned config_overlay:1;					//11
					unsigned flush_cache_command:1;				//12
					unsigned flush_cache_extension:1;			//13
				private:
					unsigned unknown5:2;						//14..15
				public:										//87
					unsigned smart_error_log:1;					//0
					unsigned smart_selftest:1;					//1
					unsigned media_serial_number:1;				//2
					unsigned media_card_pass_through:1;			//3
					unsigned streaming_feature:1;				//4
					unsigned general_purpose_log:1;				//5
					unsigned dma_fua_extension:1;				//6
					unsigned dma_queued_fua_extension:1;		//7
					unsigned world_wide_name:1;					//8
					unsigned urg_read_stream:1;					//9
					unsigned urg_write_stream:1;				//10
					unsigned timelimit_feature:1;				//11
					unsigned continuous_timelimit_feature:1;	//12
				private:
					unsigned unknown:3;							//13..15
			} enabled_features;
			struct __attribute__ ((packed))					//88
			{
				public:
					unsigned udma_mode0_supported:1;			//0
					unsigned udma_mode1_supported:1;			//1
					unsigned udma_mode2_supported:1;			//2
					unsigned udma_mode3_supported:1;			//3
					unsigned udma_mode4_supported:1;			//4
					unsigned udma_mode5_supported:1;			//5
					unsigned udma_mode6_supported:1;			//6
				private:
					unsigned unknown1:1;						//7
				public:
					unsigned udma_mode0_selected:1;				//8
					unsigned udma_mode1_selected:1;				//9
					unsigned udma_mode2_selected:1;				//10
					unsigned udma_mode3_selected:1;				//11
					unsigned udma_mode4_selected:1;				//12
					unsigned udma_mode5_selected:1;				//13
					unsigned udma_mode6_selected:1;				//14
				private:
					unsigned unknown2:1;						//15
			} udma_settings;
		private:
			uint16_t unknown8[4];							//89..92
		public:
			uint16_t pin80_cable;							//93
			struct __attribute__ ((packed))					//94
			{
				public:
					uint8_t recommend;							//0..7
					uint8_t current;							//8..15
			} vendors_acoustic_management_values;
			uint16_t stream_min_req_size;					//95
			uint16_t stream_transfer_time_dma;				//96
			uint16_t stream_access_latency;					//97
			uint16_t stream_perform_granularity[2];			//98..99
			uint64_t number_of_acc_sector_lba48;			//100..103
			uint16_t stream_transfer_time_pio;				//104
		private:
			uint16_t unknown9[22];							//105..126;
		public:
			struct __attribute__ ((packed))					//127
			{
				public:
					unsigned supported:2;						//0..1
				private:
					unsigned unknown:14;						//2..15
			} removable_media_status_notify;
		private:
			uint16_t unknown10[48];							//128..175
		public:
			char current_media_serial_number[60];			//176..205
		private:
			uint16_t unknown11[49];							//206..254
		public:
			uint8_t checksum;								//255
		private:
			uint8_t unknown12;								//255
	} i;

	/**
	 * Does a byte-exchange on some words, so the informations are easyer to use.
	 */
	void spin()
	{
		//easier to use
		#define xchgb( value ) \
			asm("xchgb %b0,%h0" \
				: "=q" ( value ) \
				: "0" ( value ) \
			)

		//byte-exchange on the serial_number
		for (int i = 10; i <= 19; i++)
		{
			xchgb( this->words[i] );
		}

		//byte-exchange on the firmware_revision and model_number
		for (int i = 23; i <= 46; i++)
		{
			xchgb( this->words[i] );
		}

		//throw it away
		#undef xchgb
	}

	/**
	 * Dumps the content of the indentify information to the logger (for debugging purpose).
	 */
	void dump()
	{
		//some marcos for easier use
		#define dmp( name, value ) \
			logger << message::MORE_VERBOSE << '\t' << #name << ":=\"" \
			<< value << "\"" << endl

		#define dmp_str( value ) \
			dmp( value , &this->i.value[0] )

		#define dmp_uns( value ) \
			dmp( value, hex << static_cast<uint8_t>( this->i.value) )

		#define dmp_hex( value ) \
			dmp( value, hex << this->i.value )

		#define dmp_dec( value ) \
			dmp( value, dec << this->i.value )

		logger << message::MORE_VERBOSE << "Drive Identify Information:" << endl;

		//dump all attributes
		dmp_uns( word0.command_packet );
		dmp_uns( word0.response_incomplete );
		dmp_uns( word0.drive_request );
		dmp_uns( word0.removeable_media );
		dmp_uns( word0.device_type );
		dmp_uns( word0.atapi_device );
		dmp_dec( num_of_log_cyl );
		dmp_hex( spec_configuration );
		dmp_dec( num_of_log_heads );
		dmp_dec( num_of_secs_per_head );
		dmp_str( serial_number );
		dmp_str( firmware_revision );
		dmp_str( model_number );
		dmp_dec( num_of_sec_transfered_by_interrupt );
		dmp_hex( trusted_computing_features );
		dmp_uns( valid.field_88 );
		dmp_uns( valid.fields_64_70 );
		dmp_hex( multible_sector_setting_valid );
		dmp_dec( number_of_acc_sector_lba28 );
		dmp_uns( multiword_dma_settings.mode0_supported );
		dmp_uns( multiword_dma_settings.mode1_supported );
		dmp_uns( multiword_dma_settings.mode2_supported );
		dmp_uns( multiword_dma_settings.mode0_selected );
		dmp_uns( multiword_dma_settings.mode1_selected );
		dmp_uns( multiword_dma_settings.mode2_selected );
		dmp_hex( pio_modes_supported );
		dmp_dec( min_multiword_cycle_time_in_nsec );
		dmp_dec( rec_multiword_cycle_time_in_nsec );
		dmp_dec( min_pio_cycle_time_in_nsec );
		dmp_dec( min_pio_IORDY_cycle_time_in_nsec );

		dmp_uns( supported_features.smart_feature );
		dmp_uns( supported_features.security_mode_feature );
		dmp_uns( supported_features.removeable_media_feature );
		dmp_uns( supported_features.power_management_feature );
		dmp_uns( supported_features.write_cache );
		dmp_uns( supported_features.look_ahead );
		dmp_uns( supported_features.release_interrupt );
		dmp_uns( supported_features.service_interrupt );
		dmp_uns( supported_features.reset_command );
		dmp_uns( supported_features.host_protected_area );
		dmp_uns( supported_features.write_buffer );
		dmp_uns( supported_features.read_buffer );
		dmp_uns( supported_features.nop_command );
		dmp_uns( supported_features.download_microcode );
		dmp_uns( supported_features.rw_dma_queued );
		dmp_uns( supported_features.cfa_feature );
		dmp_uns( supported_features.advanced_power_management );
		dmp_uns( supported_features.removeable_media_notification );
		dmp_uns( supported_features.standby_power_up );
		dmp_uns( supported_features.spinup_after_powerup );
		dmp_uns( supported_features.security_extension );
		dmp_uns( supported_features.auto_acoustic_management );
		dmp_uns( supported_features.addresses_48bit );
		dmp_uns( supported_features.config_overlay );
		dmp_uns( supported_features.flush_cache_command );
		dmp_uns( supported_features.flush_cache_extension );
		dmp_uns( supported_features.smart_error_log );
		dmp_uns( supported_features.smart_selftest );
		dmp_uns( supported_features.media_serial_number );
		dmp_uns( supported_features.media_card_pass_through );
		dmp_uns( supported_features.streaming_feature );
		dmp_uns( supported_features.general_purpose_log );
		dmp_uns( supported_features.dma_fua_extension );
		dmp_uns( supported_features.dma_queued_fua_extension );
		dmp_uns( supported_features.world_wide_name );
		dmp_uns( supported_features.urg_read_stream );
		dmp_uns( supported_features.urg_write_stream );
		dmp_uns( supported_features.timelimit_feature );
		dmp_uns( supported_features.continuous_timelimit_feature );

		dmp_uns( enabled_features.smart_feature );
		dmp_uns( enabled_features.security_mode_feature );
		dmp_uns( enabled_features.removeable_media_feature );
		dmp_uns( enabled_features.power_management_feature );
		dmp_uns( enabled_features.write_cache );
		dmp_uns( enabled_features.look_ahead );
		dmp_uns( enabled_features.release_interrupt );
		dmp_uns( enabled_features.service_interrupt );
		dmp_uns( enabled_features.reset_command );
		dmp_uns( enabled_features.host_protected_area );
		dmp_uns( enabled_features.write_buffer );
		dmp_uns( enabled_features.read_buffer );
		dmp_uns( enabled_features.nop_command );
		dmp_uns( enabled_features.download_microcode );
		dmp_uns( enabled_features.rw_dma_queued );
		dmp_uns( enabled_features.cfa_feature );
		dmp_uns( enabled_features.advanced_power_management );
		dmp_uns( enabled_features.removeable_media_notification );
		dmp_uns( enabled_features.standby_power_up );
		dmp_uns( enabled_features.spinup_after_powerup );
		dmp_uns( enabled_features.security_extension );
		dmp_uns( enabled_features.auto_acoustic_management );
		dmp_uns( enabled_features.addresses_48bit );
		dmp_uns( enabled_features.config_overlay );
		dmp_uns( enabled_features.flush_cache_command );
		dmp_uns( enabled_features.flush_cache_extension );
		dmp_uns( enabled_features.smart_error_log );
		dmp_uns( enabled_features.smart_selftest );
		dmp_uns( enabled_features.media_serial_number );
		dmp_uns( enabled_features.media_card_pass_through );
		dmp_uns( enabled_features.streaming_feature );
		dmp_uns( enabled_features.general_purpose_log );
		dmp_uns( enabled_features.dma_fua_extension );
		dmp_uns( enabled_features.dma_queued_fua_extension );
		dmp_uns( enabled_features.world_wide_name );
		dmp_uns( enabled_features.urg_read_stream );
		dmp_uns( enabled_features.urg_write_stream );
		dmp_uns( enabled_features.timelimit_feature );
		dmp_uns( enabled_features.continuous_timelimit_feature );

		dmp_uns( udma_settings.udma_mode0_supported );
		dmp_uns( udma_settings.udma_mode1_supported );
		dmp_uns( udma_settings.udma_mode2_supported );
		dmp_uns( udma_settings.udma_mode3_supported );
		dmp_uns( udma_settings.udma_mode4_supported );
		dmp_uns( udma_settings.udma_mode5_supported );
		dmp_uns( udma_settings.udma_mode6_supported );
		dmp_uns( udma_settings.udma_mode0_selected );
		dmp_uns( udma_settings.udma_mode1_selected );
		dmp_uns( udma_settings.udma_mode2_selected );
		dmp_uns( udma_settings.udma_mode3_selected );
		dmp_uns( udma_settings.udma_mode4_selected );
		dmp_uns( udma_settings.udma_mode5_selected );
		dmp_uns( udma_settings.udma_mode6_selected );

		dmp_hex( pin80_cable );
		dmp_dec( vendors_acoustic_management_values.recommend );
		dmp_dec( vendors_acoustic_management_values.current );
		dmp_dec( stream_min_req_size );
		dmp_dec( stream_transfer_time_dma );
		dmp_dec( stream_access_latency );
		dmp_hex( stream_perform_granularity[0] );
		dmp_hex( stream_perform_granularity[1] );
		dmp_dec( number_of_acc_sector_lba48 );
		dmp_dec( stream_transfer_time_pio );
		dmp_uns( removable_media_status_notify.supported );
		dmp_str( current_media_serial_number );
		dmp_hex( checksum );

		//throw the defines away
		#undef dmp_dec
		#undef dmp_hex
		#undef dmp_uns
		#undef dmp_str
		#undef dmp
	}

	/**
	 * Dumps the identify_information in raw (hex) format to the logger (for debuging purpose).
	 */
	void dump_raw()
	{
		logger << message::MORE_VERBOSE << hex;
		for ( uint32_t i = 0; i < 256; i++ )
		{
			logger << static_cast<uint8_t>( ( this->words[i] & 0xF000 ) >> 12 )
				<< static_cast<uint8_t>( ( this->words[i] & 0x0F00 ) >>  8 )
				<< static_cast<uint8_t>( ( this->words[i] & 0x00F0 ) >>  4 )
				<< static_cast<uint8_t>( ( this->words[i] & 0x000F ) );

		}
		logger << endl;
	}

	/**
	 * Checks if this information describes an atapi device
	 * @return true if this is an atapi device else false
	 */
	inline bool is_atapi() const
	{
		return ( this->i.spec_configuration == ATAPI_SPEC_CONFIGURATION );
	}

	/**
	 * Checks if this information describes an ata device
	 * @return true if this is an ata device else false
	 */
	inline bool is_ata() const
	{
		return ( this->i.spec_configuration == ATA_SPEC_CONFIGURATION );
	}

	/**
	 * Checks if this information device an device with lba 48 mode
	 * @return true if this device supports lba48
	 */
	inline bool has_lba48() const
	{
		return ( 
				( this->i.supported_features.addresses_48bit == 1 ) &&
				( this->i.number_of_acc_sector_lba48 != 0 )
			   );
	}

	/**
	 * Returns the maximum number of sectors addressable with this device
	 * @return the amount of sectors
	 */
	inline uint32_t max_sectors() const
	{
		return ( this->i.number_of_acc_sector_lba48 == 0 ) ?
			this->i.number_of_acc_sector_lba28 :
			this->i.number_of_acc_sector_lba48;
	}
};

/**
 * The possible drives to execute commands on.
 */
enum ata_drive_number
{
	ata_none,			/**< a nonexisting drive */
	ata_master,			/**< a master drive */
	ata_slave			/**< a slave drive */
};

/**
 * Describes the hd_selector port
 */
union hd_selector
{
	uint8_t bits;
	struct __attribute__ ((packed)) 
	{
		/**
		 * Unused head_selector
		 */
		unsigned head_selector:4;

		/**
		 * Selects the Master or Slave drive.
		 * 0 := master (drive 0)
		 * 1 := slave  (drive 1)
		 */
		unsigned drive_selector:1;

		private:
		unsigned unknown_bit6:1;

		public:
		/**
		 * Access Mode.
		 * 0 := CHS
		 * 1 := LBA
		 */
		unsigned mode:1;

		private:
		unsigned unknown_bit8:1;

	} config;

	hd_selector( bool master = true, bool lba_mode = true )
		: bits( 0 )
	{
		this->config.drive_selector = master ? 0 : 1;
		this->config.mode = lba_mode ? 1 : 0;
	}
};

/**
 * Describes the controller_status port
 */
union controller_status
{
	uint8_t bits;
	struct __attribute__ ((packed))
	{
		/** Error flag (when set). Send a new command to clear it 
		 * (or nuke it with a Software Reset). */
		unsigned error:1;

		private:
		unsigned unknown:2;

		public:
		/** Set when the drive has PIO data to transfer, or is ready to accept PIO data. */
		unsigned data_request:1;

		/** Overlapped Mode Service Request */
		unsigned service:1;

		/** Drive Fault Error (does not set ERR!) */
		unsigned device_fault:1;

		/** Bit is clear when drive is spun down, or after an error. Set otherwise. */
		unsigned ready:1;

		/** Drive is preparing to accept/send data -- wait until this bit clears. 
		 * If it never clears, do a Software Reset. Technically, when BSY is set, the 
		 * other bits in the Status byte are meaningless. */
		unsigned busy:1;
	} status;

	/** Constructor for a controller_status */
	controller_status(
			const bool error = false,
			const bool data_request = false,
			const bool service = false,
			const bool device_fault = false,
			const bool ready = false,
			const bool busy = false)
		: bits(0)
	{
		status.error		= error			? 1 : 0;
		status.data_request	= data_request	? 1 : 0;
		status.service		= service		? 1 : 0;
		status.device_fault	= device_fault	? 1 : 0;
		status.ready		= ready			? 1 : 0;
		status.busy			= busy			? 1 : 0;
	}

	void dump();
};

/**
 * Desribes the controller_config port
 */
union controller_config
{
	uint8_t bits;
	struct __attribute__ ((packed))
	{
		private:
			unsigned unknown_bit1:1;

		public:
			/** Set this to stop the current device from sending interrupts. */
			unsigned interrupts_disabled:1;

			/** Software Reset -- set this to reset all ATA drives on a bus, if one is 
			 * misbehaving. */
			unsigned software_reset:1;

		private:
			unsigned unknown_bits4_6:3;

		public:
			/** Set this to read back the High Order Byte of the last
			 * LBA48 value sent to an IO port. */
			unsigned high_order_byte:1;
	} config;

	/** Constructor for a controller config */
	controller_config(
			bool interrupts_disabled,
			bool software_reset,
			bool high_order_byte )
		:	bits( 0 )	
	{
		this->config.interrupts_disabled = interrupts_disabled ? 1 : 0;
		this->config.software_reset = software_reset ? 1 : 0;
		this->config.high_order_byte = high_order_byte ? 1 : 0;
	}
};

/**
 * Desribes the controller errors
 */
union error_status
{
	uint8_t bits;
	struct __attribute__ ((packed))
	{
		unsigned no_address_mark:1;
		unsigned no_track_0:1;
		unsigned abort:1;
		unsigned media_change_request:1;
		unsigned id_not_found:1;
		unsigned media_change:1;
		unsigned uncorrected_error:1;
		union
		{
			unsigned bad_block:1;
			unsigned ultra_DMA_bad_CRC:1;
		} bad;
	} ata_error;
	struct __attribute__ ((packed))
	{
		unsigned illegal_length_indication:1;
		unsigned end_of_media:1;
		unsigned command_abbort:1;
		unsigned media_change_request:1;
		private:
		unsigned unused:4;
	} atapi_arror;
};

} //namespace ata
} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* ! _KERN_ARCH_X86_ATA_ATA_DATA_STRUCTURS_H_ */
