#ifndef _KERN_ARCH_X86_CORE_SEGMENT_SELECTOR_H_
#define _KERN_ARCH_X86_CORE_SEGMENT_SELECTOR_H_

#include "x86/core/privilege.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Descripes a Segment Selector Register
 */
union segment_selector
{
	enum table
	{
		global_descriptor_table = 0,
		local_descriptor_table = 1
	};

	struct __attribute__((packed)) segment_selector_content
	{
		/**
		 * This Selector is valid for which Privilege Level.
		 * 0: ring 0 (kernel level)
		 * 1: ring 1
		 * 2: ring 2
		 * 3: ring 3 (user level)
		 */
		unsigned privilege_level:2;

		/**
		 * The belonging Segment Descriptor is in:
		 * 0: the Global Descriptor Table
		 * 1: the Local Descriptor Table
		 */
		unsigned table_indicator:1;

		/**
		 * Which is the belonging Entry in the choosen Descriptor Table.
		 */
		unsigned dt_entry_nr:13;
	} content;
	uint16_t bits;

	segment_selector(
			uint16_t entry = 0,
			privilege::level pl = privilege::system_privilege_level,
			segment_selector::table t = global_descriptor_table)
		: bits(0)
	{
		switch (pl)
		{
			case privilege::system_privilege_level:
				this->content.privilege_level= 0;
				break;
			case privilege::unused_level_1:
				this->content.privilege_level = 1;
				break;
			case privilege::unused_level_2:
				this->content.privilege_level = 2;
				break;
			case privilege::user_privilege_level:
				this->content.privilege_level = 3;
				break;
		}

		switch (t)
		{
			case global_descriptor_table:
				this->content.table_indicator = 0;
				break;
			case local_descriptor_table:
				this->content.table_indicator = 1;
				break;
		}

		this->content.dt_entry_nr = entry;
	}
}__attribute__ ((aligned (4)));

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_SEGMENT_SELECTOR_H_ */
