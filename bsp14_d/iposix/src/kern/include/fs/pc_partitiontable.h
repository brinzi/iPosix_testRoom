#ifndef _KERN_INCLUDE_FS_PC_PARTITIONTABLE_H_
#define _KERN_INCLUDE_FS_PC_PARTITIONTABLE_H_

#include <stdint.h>

namespace iposix
{
/**
 * Contains partition_type and pc_partitiontable_entrys
 */
namespace fs
{

/**
 * This Enumaration defines the most commonly used partitiontypes.
 * More Information at:
 * http://www.win.tue.nl/~aeb/partitions/partition_types-1.html
 */
enum partition_type
{
	p_unused			= 0x00,	// leer/unbenutzt
	p_fat12_floppy		= 0x01,	// FAT12 (Floppy Disks)
	p_fat16_leq_32mb	= 0x04,	// FAT16 < 32 MB
	p_ext_part			= 0x05,	// erweiterte Partition
	p_fat16_gr_32mb		= 0x06,	// FAT16 > 32 MB
	p_ntfs_or_hpfs		= 0x07,	// NTFS (Windows NT/2000/XP/Vista/7) oder HPFS (OS/2)
	p_fat32				= 0x0B,	// FAT32
	p_fat32_bios_ext	= 0x0C,	// FAT32 mit BIOS-Extensions
	p_fat16_gr_32mb_ext	= 0x0E,	// FAT16 > 32 MB mit BIOS-Extensions
	p_ext_part_bios		= 0x0F,	// erweiterte Partition mit BIOS-Extensions
	p_oem				= 0x12,	// OEM Partition fuer Konfiguration, Diagnose, BIOS-Erweiterung (fuer Microsoft-Betriebssysteme unsichtbar)
	p_dynamic			= 0x42,	// Dynamischer Datentraeger
	p_linux_swap		= 0x82,	// Linux Swap / Solaris 2.6 X86 bis Solaris 9 X86
	p_linux_native		= 0x83,	// Linux Native
	p_linux_lvm			= 0x8E,	// Linux LVM
	p_freebsd			= 0xA5,	// FreeBSD
	p_openbsd			= 0xA6,	// OpenBSD
	p_netbsd			= 0xA9	// NetBSD
};

/**
 * Describes an entry of the partition table
 * More Information at:
 * http://de.wikipedia.org/wiki/Partitionstabelle
 */
struct __attribute__ ((packed)) pc_partitiontable_entry
{
	/**
	 * The size of a sector in bytes.
	 */
	static const uint32_t SECTOR_SIZE = 512;

	/**
	 * The Offset to the Sector beginn where the Partitiontable is located at
	 */
	static const uint32_t TABLE_OFFSET_TO_SECTOR = 0x01BE;

	/**
	 * Is the Partition Bootable.
	 * 0x0 if not bootable
	 * 0x80 if bootable
	 */
	union __attribute__ ((packed))
	{
		uint8_t bits;
		bool is_bootable;
	} bootable;

	/**
	 * The first CHS of this Partition.
	 */
	uint8_t first_chs[3];

	/**
	 * The Type of this Partition
	 */
	uint8_t partition_type_bits;


	/**
	 * The Last CHS of this Partition
	 */
	uint8_t last_chs[3];

	/**
	 * The first sector of this partition (counted from beginning of the
	 * harddisk).
	 */
	uint32_t first_sector;

	/**
	 * The Length of the Harddisk in Sectors.
	 */
	uint32_t sector_count;

	/**
	 * Returns the type of the partition which this entry describes.
	 */
	partition_type type() const
	{
		return static_cast<partition_type>( this->partition_type_bits );
	}

	/**
	 * Returns the converted first_chs head number.
	 * @return the head number
	 */
	uint8_t first_chs_head()
	{
		return first_chs[0];
	}

	/**
	 * Returns the converted first_chs sector number.
	 * @return the sector number
	 */
	uint8_t first_chs_sector()
	{
		return (first_chs[1] & 0x3F);
	}

	/**
	 * Returns the converted first_chs cylinder number.
	 * @return the cylinder number
	 */
	uint16_t first_chs_cylinder()
	{
		return ((first_chs[1] & 0xC0) << 2) & (first_chs[2]);
	}

	/**
	 * Returns the converted last_chs head number.
	 * @return the head number
	 */
	uint8_t last_chs_head()
	{
		return last_chs[0];
	}

	/**
	 * Returns the converted last_chs sector number.
	 * @return the sector number
	 */
	uint8_t last_chs_sector()
	{
		return (last_chs[1] & 0x3F);
	}

	/**
	 * Returns the converted last_chs cylinder number.
	 * @return the cylinder number
	 */
	uint16_t last_chs_cylinder()
	{
		return ((last_chs[1] & 0xC0) << 2) & (last_chs[2]);
	}
};

} //namespace fs
} //namespace iposix

#endif /* !_KERN_INCLUDE_FS_PC_PARTITIONTABLE_H_ */
