#ifndef _KERN_ARCH_X86_CORE_CPU_CORE_H_
#define _KERN_ARCH_X86_CORE_CPU_CORE_H_

#include "x86/core/global_descriptor_table.h"
#include "x86/core/interrupt_descriptor_table.h"
#include "x86/core/task_state_segment.h"
#include "core/service_routine.h"
#include "utils/noncopyable.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Desribes a CPU Core
 */
class cpu_core
	: virtual public ::iposix::utils::Noncopyable
{
	public:
		cpu_core();
		~cpu_core();

		/**
		 * Does all the initilazition stuff for the current cpu.
		 */
		void init();

		void switch_interrupt_task(
				uintptr_t stack_pointer_addr,
				uintptr_t page_table_ptr );

		/**
		 * Registers an irq handler
		 * @param[in] irq the irq number
		 * @param[in] handler the handler which handles the interrupt
		 */
		void register_irq_handler(
				uint16_t irq,
				service_routine handler);

		/**
		 * Removes the current handler for an irq
		 * @param[in] irq the irq number
		 */
		void remove_irq_handler(
				uint16_t irq);

		/**
		 * Enables the current handler for an irq
		 * @param[in] irq the irq number
		 */
		void enable_irq_handler(
				uint16_t irq);

		/**
		 * Disables the current handler for an irq
		 * @param[in] irq the irq number
		 */
		void disable_irq_handler(
				uint16_t irq);

		/**
		 * Disable or Enables the registered handler in hardware mode. Note: use
		 * this if you don't want to be notified (i.e. bogus-irqs).
		 * @param[in] irq the irq number
		 * @param[in] present the status (true for enabled else false)
		 */
		void set_irq_usability(
				int entry_number, bool present );

		/**
		 * Performs a switch to the user mode
		 * This funktion is mainly for testing purpose
		 */
		static void switch_to_user_mode();

		/**
		 * Returns the current cpu time stamp
		 * @return the time stamp
		 */
		static uint64_t get_current_cpu_time_stamp();

	private:

		/**
		 * Contains the Global Descriptor Table. This table descripes the memory
		 * segmentation and the access rights which a task in specific level has
		 * got.
		 */
		global_descriptor_table gdt;

		/**
		 * Containt sthe Interrupt Descriptor Table. This tabel
		 * descripes the association between IRQs and Exceptions and
		 * the interrupt service routines.
		 */
		interrupt_descriptor_table idt;

		/**
		 * This a the default Task State Segment used by interupt hanlders
		 */
		task_state_segment interrupt_task_tss;

		/**
		 * Activating the initial task_state_segment
		 */
		inline void init_task_state_segment();

		/**
		 * Activating the initial task_state_segment
		 */
		inline void activate_task_state_segment();
};

} //namespace x86
} //namespace arch
} //namespace iposix


#endif /* !_KERN_ARCH_X86_CORE_CPU_CORE_H_ */
