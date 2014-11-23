#ifndef _KERN_ARCH_X86_CORE_MEMORY_DESCRIPTOR_H_
#define _KERN_ARCH_X86_CORE_MEMORY_DESCRIPTOR_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Used for local and global descriptors
 */
struct __attribute__ ((packed)) memory_descriptor
{
	public:
		enum segment_type
		{
			null_pointer_segment_descriptor,
			kernel_code_segment_descriptor,
			kernel_data_segment_descriptor,
			user_code_segment_descriptor,
			user_data_segment_descriptor,
			task_state_segment_descriptor,
			local_descriptor_table_descriptor
		};

		/**
		 * The lower part of the limit.
		 */
		uint16_t limit_0_to_15;

		/**
		 * The lower part of the base address.
		 */
		uint16_t base_0_to_15;

		/**
		 * The middle part of the base address.
		 */
		uint8_t base_16_to_23;

		/**
		 * The type of this segment.
		 * TODO: find out
		 */
		unsigned type:4;

		/**
		 * Does this segment contains kernel data structures or
		 * normal code or data structures.
		 * 0 => system structures - like TSS and LDT
		 * 1 => normal code or data structures
		 */
		unsigned system:1;

		/**
		 * In which privilege level are processes allowed to access
		 * this segment.
		 */
		unsigned discriptor_privilege_level:2;

		/**
		 * Is the segment present in main memory.
		 */
		unsigned segment_present:1;

		/**
		 * The upper 4bit of the limit. It denotes the length of the
		 * Segment. The interpretation of this value depend on the
		 * *granularity* of the units.
		 */
		unsigned limit_16_to_19:4;

		/**
		 * TODO: find out
		 */
		unsigned AVL:1;

		/**
		 * Reserved is always set to 0
		 */
		unsigned reserved:1; //always 0

		/**
		 * Does this segment contains Code or Data.
		 * It affects the offset used on the segment Address.
		 * 0 => 16bit Offsets
		 * 1 => 32bit Offsets
		 * TODO: find out more
		 */
		unsigned data_or_code:1;

		/**
		 * Whether the descriptor descripes 1Byte Units or 4KiB Units.
		 * 0 => 1Byte Units
		 * 1 => 4096 Byte Units = 4KiB Units
		 */
		unsigned granularity:1;

		/**
		 * A linear Address that contains the first byte of the segment.
		 */
		uint8_t base_24_to_31;

		memory_descriptor(
				segment_type type = kernel_data_segment_descriptor,
				bool present = false,
				uint32_t base = 0x00000000,
				uint32_t limit = 0xFFFFF,
				bool high_granularity = false
				);

		uint32_t base();

		uint32_t limit();

		void dump();
	private:
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_MEMORY_DESCRIPTOR_H_ */
