#ifndef _KERN_ARCH_X86_MULTIBOOT_MULTIBOOT_H_
#define _KERN_ARCH_X86_MULTIBOOT_MULTIBOOT_H_

#ifdef ASM /* Only include this part if we in assembler code */

	/* The magic number for the Multiboot header. */
	#define MULTIBOOT_HEADER_MAGIC          0x1BADB002

	/* The magic number passed from a Multiboot compatible Bootloader */
	#define MULTIBOOT_BOOTLOADER_MAGIC		0x2BADB002

	/* The Boot-Loader should load the kernel page aligned */
	#define MULTIBOOT_PAGE_ALIGN 			1<<0

	/* The Boot-Loader should give us information about the memory */
	#define MULTIBOOT_MEMORY_INFO			1<<1

	/* Additional Informations musst be given to the boot loader if */
	/* this is not an ELF file */
	#define MULTIBOOT_AOUT_KLUDGE			1<<16

	/* The flags for the Multiboot header. */
	#ifdef __ELF__
		#define MULTIBOOT_HEADER_FLAGS		\
			(								\
				MULTIBOOT_PAGE_ALIGN |		\
				MULTIBOOT_MEMORY_INFO		\
			)
	#else
		#define MULTIBOOT_HEADER_FLAGS		\
			(								\
				MULTIBOOT_PAGE_ALIGN |		\
				MULTIBOOT_MEMORY_INFO |		\
				MULTIBOOT_AOUT_KLUDGE 		\
			)
	#endif

	/* The Checksum used for validating the Multiboot header */
	#define MULTIBOOT_CHECKSUM 				\
			-(								\
				MULTIBOOT_HEADER_MAGIC +	\
				MULTIBOOT_HEADER_FLAGS		\
			)
	
	/* The size of our stack (4KB). */
	#define STACK_SIZE                      0x1000

#endif /* ASM */

#ifndef ASM /* Only include this part if we not in assembler code */
#include "utils/singleton.hpp"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Stuff needed for Multiboot.
 * For information see
 * http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Example-OS-code
 */
class multiboot
	: virtual public ::iposix::utils::Singleton< multiboot >
{
	friend class ::iposix::utils::Singleton< multiboot >;

	public:
		void set_info(uint32_t multiboot_magic, uint32_t multiboot_info_addr);

		bool multiboot_info_valid() const;
		
		bool mem_field_present() const;
		bool boot_device_field_present() const;
		bool cmdline_field_present() const;
		bool modules_field_present() const;
		bool aout_symbol_table_field_present() const;
		bool elf_section_field_present() const;
		bool memory_map_field_present() const;
		bool drives_field_present() const;
		bool config_table_field_present() const;
		bool boot_loader_name_field_present() const;
		bool apm_table_present() const;
		bool vbe_control_field_present() const;

		/**
		 * Descripes Modules loaded at boot.
		 */
		struct __attribute__ ((packed)) module
		{
			/**
			 * Start Address of the Boot Module.
			 */
			uint32_t mod_start;

			/**
			 * End Address of the Boot Module.
			 */
			uint32_t mod_end;

			/**
			 * Name of the Boot Module.
			 */
			uint32_t string; //TODO maybe char*

			/**
			 * Reserved.
			 */
			uint32_t reserved;
		};

		/**
		 * Descripes the Symboltable of the a.out kernel.
		 */
		struct __attribute__ ((packed)) aout_symbol_table
		{
			
			/**
			 * Size of the symbol table.
			 */
			uint32_t tabsize;

			/**
			 * Size of the string after the symbol table.
			 */
			uint32_t strsize;

			/**
			 * Address of the symbol table of the a.out kernel.
			 */
			uint32_t addr;	//TODO maybe char*

			/**
			 * Reserved.
			 */
			uint32_t reserved;
		};

		/**
		 * Desripes the section header of the elf kernel.
		 */
		struct __attribute__ ((packed)) elf_section_header_table
		{
			 /**
			  * The number of entrys.
			  */
			 uint32_t num;

			 /**
			  * The size of each entry.
			  */
			 uint32_t size;

			 /**
			  * The Address of the string table.
			  */
			 uint32_t addr;

			 /**
			  * The address of the section header index.
			 */
			 uint32_t shndx;
		};

		struct __attribute__ ((packed)) memory_map
		{
			uint32_t size;
			uint32_t base_addr_low;
			uint32_t base_addr_high;
			uint32_t length_low;
			uint32_t length_high;
			uint32_t type;
		};

		struct __attribute__ ((packed)) drive_map
		{
			uint32_t size;
			uint8_t drive_number;
			uint8_t drive_mode;
			uint16_t drive_cylinders;
			uint8_t drive_heads;
			uint8_t drive_sectors;
			uint16_t drive_ports; //TODO could be more then one
		};

		/**
		 * Desripes the Advanced Power Managment Table (APM) BIOS Interface.
		 * See http://www.microsoft.com/hwdev/busbios/amp_12.htm for more
		 * details.
		 */
		struct __attribute__ ((packed)) apm_table
		{
			uint16_t version;
			uint16_t cseg;
			uint32_t offset;
			uint16_t cseg_16;
			uint16_t dseg;
			uint16_t flags;
			uint16_t cseg_len;
			uint16_t cseg_16_len;
			uint16_t dseg_len;
		};

		uint32_t get_memory_map_length();
		memory_map* get_memory_map();

		uint32_t get_memory_lower();
		uint32_t get_memory_upper();

		uint32_t get_drive_map_length() const;
		drive_map* get_drive_map() const;

		/**
		 * Descripes the Multiboot Information passed to the kernel
		 */
		struct __attribute__ ((packed)) multiboot_info
		{
			/**
			 * The first longword indicates the presence and validity of other
			 * fields in the Multiboot information structure. All
			 * as-yet-undefined bits must be set to zero by the boot loader. Any
			 * set bits that the operating system does not understand should be
			 * ignored. Thus, the `flags' field also functions as a version
			 * indicator, allowing the Multiboot information structure to be
			 * expanded in the future without breaking anything.
			 */
			uint32_t flags;

			/**
			 * If bit 0 in the `flags' word is set, then the `mem_*' fields are
			 * valid. `mem_lower' indicate the amount of lower memory,
			 * respectively, in kilobytes. Lower memory starts at address 0, and
			 * upper memory starts at address 1 megabyte. The maximum possible
			 * value for lower memory is 640 kilobytes.
			 */
			uint32_t mem_lower;							// if flags[0] is set

			/**
			 * If bit 0 in the `flags' word is set, then the `mem_*' fields are
			 * valid. `mem_upper' indicate the amount of upper memory,
			 * respectively, in kilobytes. Upper memory starts at address 1 megabyte.  
			 * The value returned for upper memory is maximally the address of
			 * the first upper memory hole minus 1 megabyte. It is not
			 * guaranteed to be this value.
			 */
			uint32_t mem_upper;							// if flags[0] is set

			/**
			 * If bit 1 in the `flags' word is set, then the `boot_device' field
			 * is valid, and indicates which bios disk device the boot loader
			 * loaded the OS image from. If the OS image was not loaded from a
			 * bios disk, then this field must not be present (bit 3 must be
			 * clear). The operating system may use this field as a hint for
			 * determining its own root device, but is not required to. The
			 * `boot_device' field is laid out in four one-byte subfields as
			 * follows:
			 *
			 *    +-------+-------+-------+-------+
			 *    | part3 | part2 | part1 | drive |
			 *    +-------+-------+-------+-------+
			 *
			 * The first byte contains the bios drive number as understood by
			 * the bios INT 0x13 low-level disk interface: e.g. 0x00 for the
			 * first floppy disk or 0x80 for the first hard disk. 
			 *
			 * The three remaining bytes specify the boot partition. `part1'
			 * specifies the top-level partition number, `part2' specifies a
			 * sub-partition in the top-level partition, etc. Partition numbers
			 * always start from zero. Unused partition bytes must be set to
			 * 0xFF. For example, if the disk is partitioned using a simple
			 * one-level DOS partitioning scheme, then `part1' contains the DOS
			 * partition number, and `part2' and `part3' are both 0xFF. As
			 * another example, if a disk is partitioned first into DOS
			 * partitions, and then one of those DOS partitions is subdivided
			 * into several BSD partitions using BSD's disklabel strategy, then
			 * `part1' contains the DOS partition number, `part2' contains the
			 * BSD sub-partition within that DOS partition, and `part3' is 0xFF. 
			 *
			 * DOS extended partitions are indicated as partition numbers
			 * starting from 4 and increasing, rather than as nested
			 * sub-partitions, even though the underlying disk layout of
			 * extended partitions is hierarchical in nature. For example, if
			 * the boot loader boots from the second extended partition on a
			 * disk partitioned in conventional DOS style, then `part1' will be
			 * 5, and `part2' and `part3' will both be 0xFF. 
			 */
			uint32_t boot_device;						// if flags[1] is set

			/**
			 * If bit 2 of the `flags' longword is set, the `cmdline' field is
			 * valid, and contains the physical address of the command line to
			 * be passed to the kernel. The command line is a normal C-style
			 * zero-terminated string.
			 */
			char* cmdline;							// if flags[2] is set

			
			/**
			 * If bit 3 of the `flags' is set, then the `mods' fields indicate
			 * to the kernel what boot modules were loaded along with the kernel
			 * image, and where they can be found. `mods_count' contains the
			 * number of modules loaded.`mods_count' may be zero, indicating no
			 * boot modules were loaded, even if bit 1 of `flags' is set.
			 */
			uint32_t mods_count;						// if flags[3] is set

			/**
			 * If bit 3 of the `flags' is set, then the `mods' fields indicate
			 * to the kernel what boot modules were loaded along with the kernel
			 * image, and where they can be found. `mods_addr' contains the
			 * physical address of the first module structure.
			 */
			module* mods_addr;							// if flags[3] is set

			/**
			 * If bit 4 in the `flags' word is set, then the a.out structure is
			 * valid.
			 * If bit 5 in the `flags' word is set, then the ELF structure is
			 * valid.
			 */
			union
			{
				aout_symbol_table aout_sym;				// if flags[4] is set
				elf_section_header_table elf_sec;		// if flags[5] is set
			} syms;

			/**
			 * If bit 6 in the `flags' word is set, then the `mmap_*' fields are
			 * valid, and indicate the address and length of a buffer containing
			 * a memory map of the machine provided by the bios.
			 * `mmap_addr' is the address, 
			 */
			uint32_t mmap_length;						// if flags[6] is set

			/**
			 * If bit 6 in the `flags' word is set, then the `mmap_*' fields are
			 * valid, and indicate the address and length of a buffer containing
			 * a memory map of the machine provided by the bios.
			 * `mmap_length' is the total size of the buffer. 
			 */
			memory_map* mmap_addr;							// if flags[6] is set

			/**
			 * If bit 7 in the `flags' is set, then the `drives_*' fields are
			 * valid, and indicate the address of the physical address of the
			 * first drive structure and the size of drive structures.
			 * `drives_length' is the total size of drive structures.
			 */
			uint32_t drives_length;						// if flags[7] is set

			/**
			 * If bit 7 in the `flags' is set, then the `drives_*' fields are
			 * valid, and indicate the address of the physical address of the
			 * first drive structure and the size of drive structures.
			 * `drives_addr' is the address, 
			 */
			drive_map* drives_addr;						// if flags[7] is set

			/**
			 * If bit 8 in the `flags' is set, then the `config_table' field is
			 * valid, and indicates the address of the rom configuration table
			 * returned by the GET CONFIGURATION bios call. If the bios call
			 * fails, then the size of the table must be zero.
			 */
			uint32_t config_table_addr;					// if flags[8] is set

			/**
			 * If bit 9 in the `flags' is set, the `boot_loader_name' field is
			 * valid, and contains the physical address of the name of a boot
			 * loader booting the kernel. The name is a normal C-style
			 * zero-terminated string. 
			 */
			char* boot_loader_name;						// if flags[9] is set

			/**
			 * If bit 10 in the `flags' is set, the `apm_table' field is valid,
			 * and contains the physical address of an apm table.
			 */
			apm_table* apm_addr;						// if flags[10] is set

			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The fields `vbe_control_info' and `vbe_mode_info' contain the
			 * physical addresses of vbe control information returned by the vbe
			 * Function 00h and vbe mode information returned by the vbe
			 * Function 01h, respectively. 
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_control_info;					// if flags[11] is set
			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The fields `vbe_control_info' and `vbe_mode_info' contain the
			 * physical addresses of vbe control information returned by the vbe
			 * Function 00h and vbe mode information returned by the vbe
			 * Function 01h, respectively. 
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_mode_info;						// if flags[11] is set

			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The field `vbe_mode' indicates current video mode in the format
			 * specified in vbe 3.0. 
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_mode;							// if flags[11] is set

			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The rest fields `vbe_interface_seg', `vbe_interface_off', and
			 * `vbe_interface_len' contain the table of a protected mode
			 * interface defined in vbe 2.0+. If this information is not
			 * available, those fields contain zero. Note that vbe 3.0 defines
			 * another protected mode interface which is incompatible with the
			 * old one. If you want to use the new protected mode interface, you
			 * will have to find the table yourself.
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_interface_seg;					// if flags[11] is set

			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The rest fields `vbe_interface_seg', `vbe_interface_off', and
			 * `vbe_interface_len' contain the table of a protected mode
			 * interface defined in vbe 2.0+. If this information is not
			 * available, those fields contain zero. Note that vbe 3.0 defines
			 * another protected mode interface which is incompatible with the
			 * old one. If you want to use the new protected mode interface, you
			 * will have to find the table yourself.
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_interface_off;					// if flags[11] is set

			/**
			 * If bit 11 in the `flags' is set, the graphics table is available.
			 * This must only be done if the kernel has indicated in the
			 * `Multiboot Header' that it accepts a graphics mode.
			 *
			 * The rest fields `vbe_interface_seg', `vbe_interface_off', and
			 * `vbe_interface_len' contain the table of a protected mode
			 * interface defined in vbe 2.0+. If this information is not
			 * available, those fields contain zero. Note that vbe 3.0 defines
			 * another protected mode interface which is incompatible with the
			 * old one. If you want to use the new protected mode interface, you
			 * will have to find the table yourself.
			 * 
			 * The fields for the graphics table are designed for vbe, but
			 * Multiboot boot loaders may simulate vbe on non-vbe modes, as if
			 * they were vbe modes. 
			 */
			uint32_t vbe_interface_len;					// if flags[11] is set
		};

		/* The magic number passed by a Multiboot-compliant boot loader. */
		static const uint32_t MULTIBOOT_BOOTLOADER_MAGIC = 0x2BADB002;

		const multiboot_info * mb_info;

	private:
		multiboot();

		~multiboot();

		uint32_t mb_magic;

};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* ! ASM */

#endif /* !_KERN_ARCH_X86_MULTIBOOT_MULTIBOOT_H_ */
