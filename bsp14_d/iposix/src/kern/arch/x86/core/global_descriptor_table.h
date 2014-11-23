#ifndef _KERN_ARCH_X86_CORE_GLOBAL_DESCRIPTOR_TABLE_H_
#define _KERN_ARCH_X86_CORE_GLOBAL_DESCRIPTOR_TABLE_H_

#include "x86/core/memory_descriptor.h"
#include "utils/noncopyable.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/* Predefine */
class task_state_segment;

/**
 * Descripes the Global Descriptor Table Register.
 */
struct __attribute__ ((packed)) global_descriptor_table_register
{
	public:
		/**
		 * The number of entrys in the Global Descriptor Table.
		 */
		uint16_t limit;

		/**
		 * The Address of the Global Descriptor Table.
		 */
		memory_descriptor* base;

		global_descriptor_table_register()
			:	limit( 0 ), base( 0 ), unused( 0 )
		{ };

		global_descriptor_table_register(
				uint16_t entries,
				memory_descriptor* md )
			:	limit( ( sizeof( memory_descriptor ) * entries ) - 1 ), 
				base( md ), unused( 0 )
		{ };

	private:
		/**
		 * Just for being aligned
		 */
		uint16_t unused;
} __attribute__ ((aligned (4)));

class global_descriptor_table
	: virtual public ::iposix::utils::Noncopyable
{
	public:
		global_descriptor_table();
		~global_descriptor_table();

		static const uint16_t NULL_POINTER_SEGMENT_NR = 0;
		static const uint16_t GLOBAL_KERNEL_CODE_SEGMENT_NR  = 1;
		static const uint16_t GLOBAL_KERNEL_DATA_SEGMENT_NR  = 2;
		static const uint16_t GLOBAL_USER_CODE_SEGMENT_NR = 3;
		static const uint16_t GLOBAL_USER_DATA_SEGMENT_NR = 4;
		static const uint16_t INTERRUPT_TASK_TSS_NR = 5;
		static const uint16_t INTERRUPT_TASK_LDT_NR = 6;
		static const uint16_t FIRST_FREE_SEGMENT = 5;

		/**
		 * Capacity of the GDT
		 * NOTE: maximum 8192
		 */
		static const uint16_t GDT_SIZE = 16;

		/**
		 * Sets a entry to describe a task state segment
		 * @param[in] n entry number
		 * @param[in] tss pointer to the tss to set
		 */
		void set_task_state_segment(
				uint16_t n, 
				const task_state_segment* tss );

		/**
		 * Set up a default Local Descriptor Table segment
		 */
		void set_local_descriptor_table_segment( const uint16_t n );

		/**
		 * Initializes this GDT on the current CPU.
		 */
		void init_gdt();

		/**
		 * Activates this GDT on the current CPU.
		 */
		void activate_gdt();

		void activate_ldt( const uint16_t n );

		void dump();

	private:

		/**
		 * The Register content for this GDT.
		 */
		global_descriptor_table_register reg;


		/**
		 * Capacity of the default Local Descriptor Table.
		 */
		static const uint16_t DEFAULT_LDT_SIZE = 1;

		/**
		 * Contains the Global Descriptor Table. This table descripes the memory
		 * segmentation and the access rights which a task in specific level has
		 * got.
		 */
		memory_descriptor table[GDT_SIZE];

		/**
		 * This is a default Local Descriptor Table. This table descripes the
		 * memory segmentation and the access rights which a task in specific
		 * level has got (see Global Descriptor Table). This table is not used
		 * by modern Operation Systems but must be initialized. This table is
		 * only nessecary for legaccy support.
		 */
		memory_descriptor default_ldt[DEFAULT_LDT_SIZE];

		/**
		 * This method creates an entry in the Global Descriptor Table for
		 * disallowing the access to the Address 0x0 (the Null Pointer).
		 */
		void set_null_pointer_segment(
				uint16_t n = NULL_POINTER_SEGMENT_NR );

		/**
		 * This Method creates an entry in the Global Descriptor Table for
		 * accessing the whole memory as kernel code.
		 */
		void set_global_kernel_code_segment();

		/**
		 * This Method creates an entry in the Global Descriptor Table for
		 * accessing the whole memory as kernel data.
		 */
		void set_global_kernel_data_segment();

		/**
		 * This Method creates an entry in the Global Descriptor Table for
		 * accessing the whole memory as user code.
		 */
		void set_global_user_code_segment();

		/**
		 * This Method creates an entry in the Global Descriptor Table for
		 * accessing the whole memory as user data.
		 */
		void set_global_user_data_segment();
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_GLOBAL_DESCRIPTOR_TABLE_H_ */
