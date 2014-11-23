#ifndef _KERN_ARCH_X86_CORE_LOCAL_DESCRIPTOR_TABLE_H_
#define _KERN_ARCH_X86_CORE_LOCAL_DESCRIPTOR_TABLE_H_

#include "x86/core/memory_descriptor.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

class local_descriptor_table
{
	public:
		local_descriptor_table();
		local_descriptor_table(uint16_t size);
		~local_descriptor_table();

		void activate();

	private:
		/**
		 * Capacity of the default Local Descriptor Table.
		 */
		static const uint16_t DEFAULT_LDT_SIZE = 1;


		/**
		 * Descripes the Local Descriptor Table Register.
		 */
		struct __attribute__ ((packed)) local_descriptor_table_register
		{
			/**
			 * Descripes which entry of the GDT points to the current tasks
			 * Local Descriptor Table.
			 */
			uint16_t segment_nr;
		};

		local_descriptor_table_register reg;

		/**
		 * This is a default Local Descriptor Table. This table descripes the
		 * memory segmentation and the access rights which a task in specific
		 * level has got (see Global Descriptor Table). This table is not used
		 * by modern Operation Systems but must be initialized. This table is
		 * only nessecary for legaccy support.
		 */
		memory_descriptor *table;

		/**
		 * This is a default Local Descriptor Table. This table descripes the
		 * memory segmentation and the access rights which a task in specific
		 * level has got (see Global Descriptor Table). This table is not used
		 * by modern Operation Systems but must be initialized. This table is
		 * only nessecary for legaccy support.
		 */
		static memory_descriptor default_table[DEFAULT_LDT_SIZE];
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_LOCAL_DESCRIPTOR_TABLE_H_ */
