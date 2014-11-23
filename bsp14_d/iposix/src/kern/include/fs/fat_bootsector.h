#ifndef _KERN_INCLUDE_FS_FAT_BOOTSECTOR_H_
#define _KERN_INCLUDE_FS_FAT_BOOTSECTOR_H_

#include <stdint.h>

namespace iposix {
namespace fs {

/**
 * Describes the additional fat12/16 information.
 */
struct __attribute__((packed)) fat_12_16_info_t
{
	public:
		uint8_t physical_drive_number;					//36
	private:
		uint8_t reserved;								//37
	public:
		uint8_t ext_bootsignature;						//38
		uint32_t serial_number;							//39..42
		char label[11];									//43..53
		char fat_variant[8];							//54..61
		uint8_t bootloader[448];						//62..509
};

/**
 * Describes the additional fat32 information
 */
struct __attribute__((packed)) fat_32_info_t
{
	public:
		uint32_t sectors_per_fat;						//36..39
		uint16_t fat_flags;								//40..41
		uint16_t fat_32_version;						//42..43
		uint32_t rootdir_cluster;						//44..47
		uint16_t fs_information_sector;					//48..49
		uint16_t bootsector_copy_sector;				//50..51
	private:
		uint8_t reserved1[12];							//52..63
	public:
		uint8_t physical_drive_number;					//64
	private:
		uint8_t reserved2;								//65
	public:
		uint8_t ext_bootsignature;						//66
		uint32_t serial_number;							//67..70
		char label[11];									//71..81
		char fat_variant[8];							//82..89
		uint8_t bootloader[420];						//90..509
};

/**
 * Describes the fat bootsector
 */
struct __attribute__((packed)) fat_bootsector_t
{
	public:
		uint8_t maschinecode[3];							//0..2
		char oem_name[8];									//3..10
		uint16_t bytes_per_sector;							//11..12
		uint8_t sector_per_cluster;							//13
		uint16_t reserved_sectors;							//14..15
		uint8_t fat_copies;									//16
		uint16_t max_dir_entrys_in_root;					//17..18
		uint16_t sectors_old;								//19..20
		uint8_t media_descriptor;							//21
		uint16_t sectors_per_fat;							//22..23
		uint16_t sectors_per_track;							//24..25
		uint16_t pages_or_heads;							//26..27
		uint32_t hidden_sectors;							//28..31
		uint32_t sectors;									//32..35
		union __attribute__((packed))						//36..510
		{
			fat_12_16_info_t fat12_info;
			fat_12_16_info_t fat16_info;
			fat_32_info_t fat32_info;
		};
		uint16_t bios_bootsignature;						//510..511

		void dump( fat_type_t as_type ) const;
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_FAT_BOOTSECTOR_H_ */
