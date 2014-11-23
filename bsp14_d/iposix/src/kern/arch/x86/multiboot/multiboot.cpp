#include "multiboot.h"

namespace iposix
{
namespace arch
{
namespace x86
{

multiboot::multiboot()
:	mb_info(),
	mb_magic(0)
{ }

multiboot::~multiboot()
{ }

void multiboot::set_info(uint32_t multiboot_magic, uint32_t multiboot_info_addr)
{
	this->mb_magic = multiboot_magic;
	this->mb_info = reinterpret_cast<multiboot_info*>(multiboot_info_addr);
}

bool multiboot::multiboot_info_valid() const
{
	return (this->mb_magic == multiboot::MULTIBOOT_BOOTLOADER_MAGIC);
}

bool multiboot::mem_field_present() const
{
	return (this->mb_info->flags & 0x1);
}

bool multiboot::boot_device_field_present() const
{
	return (this->mb_info->flags & 0x2);
}

bool multiboot::cmdline_field_present() const
{
	return (this->mb_info->flags & 0x4);
}

bool multiboot::modules_field_present() const
{
	return (this->mb_info->flags & 0x8);
}

bool multiboot::aout_symbol_table_field_present() const
{
	return (this->mb_info->flags & 0x10);
}

bool multiboot::elf_section_field_present() const
{
	return (this->mb_info->flags & 0x20);
}

bool multiboot::memory_map_field_present() const
{
	return (this->mb_info->flags & 0x40);
}

bool multiboot::drives_field_present() const
{
	return (this->mb_info->flags & 0x80);
}

bool multiboot::config_table_field_present() const
{
	return (this->mb_info->flags & 0x100);
}

bool multiboot::boot_loader_name_field_present() const
{
	return (this->mb_info->flags & 0x200);
}

bool multiboot::apm_table_present() const
{
	return (this->mb_info->flags & 0x400);
}

bool multiboot::vbe_control_field_present() const
{
	return (this->mb_info->flags & 0x800);
}

multiboot::memory_map* multiboot::get_memory_map()
{
	return this->mb_info->mmap_addr;
}

uint32_t multiboot::get_memory_map_length()
{
	return this->mb_info->mmap_length;
}

uint32_t multiboot::get_memory_lower()
{
	return this->mb_info->mem_lower;
}

uint32_t multiboot::get_memory_upper()
{
	return this->mb_info->mem_upper + (1024 * 1024);
}

uint32_t multiboot::get_drive_map_length() const
{
	return this->mb_info->drives_length;
}

multiboot::drive_map* multiboot::get_drive_map() const
{
	return this->mb_info->drives_addr;
}

extern "C" void multiboot_init(uint32_t multiboot_magic, uint32_t multiboot_info_addr)
{
	::iposix::utils::Singleton< multiboot >::instance().set_info(
			multiboot_magic, 
			multiboot_info_addr
			);
}

} //namespace x86
} //namespace arch
} //namespace iposix
