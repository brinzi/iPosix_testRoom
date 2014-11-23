#ifndef _KERN_ARCH_X86_CORE_INTERRUPT_DESCRIPTOR_TABLE_H_
#define _KERN_ARCH_X86_CORE_INTERRUPT_DESCRIPTOR_TABLE_H_

#include "x86/core/privilege.h"
#include "x86/core/gate_descriptor.h"
#include "x86/core/isr.h"
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
 * Describes the interrupt descriptor table register
 */
struct __attribute__ ((packed)) interrupt_descriptor_table_register
{
	public:
		/** Limit aka size */
		uint16_t limit;
		/** a pointer to the table */
		gate_descriptor* base;

		/** Constructor */
		interrupt_descriptor_table_register(
				uint16_t entries,
				gate_descriptor* gt)
		:	limit( ( sizeof( gate_descriptor ) * entries ) - 1), 
			base( gt ), unused( 0 )
		{ };

	private:
		uint16_t unused;
};

/**
 * Descripes the Interrupt Descriptor Table.
 */
class interrupt_descriptor_table
	: virtual public ::iposix::utils::Noncopyable
{
	public:
		/**
		 * Creates an Interrupt Descriptor Table.
		 */
		interrupt_descriptor_table();

		/** Desctructor */
		~interrupt_descriptor_table();

		/** The number of processor exceptions */
		static const uint16_t EXCEPTIONs = 18;

		/** The number of hardware IRQs */
		static const uint16_t IRQs = 16;

		/**
		 * Initializes the Intertupr Descriptor Table.
		 */
		void init_idt();

		/**
		 * Registers this Interrupt Descriptor Table.
		 */
		void activate_idt();

		/**
		 * Registers an irq handler
		 * @param[in] irq the irq number
		 * @param[in] handler the handler which handles the interrupt
		 */
		static void register_irq_handler(
				uint16_t irq,
				service_routine handler);

		/**
		 * Removes the current handler for an irq
		 * @param[in] irq the irq number
		 */
		static void remove_irq_handler(
				uint16_t irq);

		/**
		 * Enables the registered handler for an irq
		 * @param[in] irq the irq number
		 */
		static void enable_irq_handler(
				uint16_t irq);

		/**
		 * Disables the registered handler for an irq
		 * @param[in] irq the irq number
		 */
		static void disable_irq_handler(
				uint16_t irq);

		/**
		 * Disable or Enables the registered handler in hardware mode. Note: use
		 * this if you don't want to be notified (i.e. bogus-irqs).
		 * @param[in] irq the irq number
		 * @param[in] present the status (true for enabled else false)
		 */
		void set_irq_usability(
				uint16_t entry_number, bool present );

		/**
		 * Dispatches an interrupt to the service routine and therefore the
		 * handler.
		 */
		static void dispatch_interrupt(isr_registers& regs);

	private:
		/** The register used for setting the IDT */
		interrupt_descriptor_table_register reg;

		/** The size of the IDT */
		static const uint16_t IDT_SIZE = 256;

		/** First irq used by the hardhware */
		static const uint16_t FIRST_IRQ = 32;

		/** System gate number aka syscall interrupt */
		static const uint16_t SYSTEM_GATE = 0x80; //0x80 := 128 ;)
		#define SYSTEM_GATE_ISR asm_isr_128

		/**
		 * Containts the Interrupt Handlers
		 */
		static service_routine irq_handler[IRQs];

		/**
		 * Contains all Interrupt Descriptors.
		 */
		gate_descriptor table[IDT_SIZE];

		/**
		 * Service routine for unknown interrupts. Calls the dummy dummy
		 * handler.
		 */
		static const service_routine STANDARD_SERVICE_ROUTINE;

		/**
		 * Service routine to the syscall handler
		 */
		static const service_routine SYSCALL_SERVICE_ROUTINE;

		/**
		 * Service routine to the exception handler
		 */
		static const service_routine EXCEPTION_SERVICE_ROUTINE;

		/**
		 * A handler which calls the kernels syscall dispatcher
		 */
		static void syscall_handler( isr_registers& regs );

		/**
		 * A handler which calls the kernels exception dispatcher
		 */
		static void exception_handler( isr_registers& regs );

		/**
		 * A dummy handler which is used for initilized but unbounded
		 * handling
		 */
		static void dummy_interrupt_handler( isr_registers& regs );

		/**
		 * A dummy dummy handler which is the default (uninitilized)
		 */
		static void dummy_dummy_handler( isr_registers& regs );

		/**
		 * Sets up an interrupt gate
		 * @param[in] entry_number the interrupt number (table index)
		 * @param[in] fct_ptr a funtion pointer to the handling function
		 */
		void set_interrupt_gate(int entry_number, gate_ptr fct_ptr);

		/**
		 * Sets up a system gate (aka syscall intertrup)
		 * @param[in] entry_number the syscall number (table index)
		 * @param[in] fct_ptr a funtion pointer to the handling function
		 */
		void set_system_gate(int entry_number, gate_ptr fct_ptr);

		/**
		 * Sets up a Trap Gate
		 * @param[in] entry_number the trap number (table index)
		 * @param[in] fct_ptr a funtion pointer to the handling function
		 */
		void set_trap_gate(int entry_number, gate_ptr fct_ptr);
};

} //namespace x86
} //namespace arch
} //namespace iposix


#endif /* !_KERN_ARCH_X86_CORE_INTERRUPT_DESCRIPTOR_TABLE_H_ */
