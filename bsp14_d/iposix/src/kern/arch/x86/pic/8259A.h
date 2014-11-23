#ifndef _KERN_ARCH_X86_PIC_8259A_H_
#define _KERN_ARCH_X86_PIC_8259A_H_

#include "x86/io_ports/io_ports.h"
#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Describes the intel(r) 8259a programmable_interrupt_controller
 */
class pic_8259a
{
	public:
		/**
		 * Relocates the interrupts.
		 * @param[in] base_isr_number_master the interrupt number used by the
		 * first irq in master PIC
		 * @param[in] base_isr_number_slave the interrupt number used by the
		 * first irq in slave PIC
		 * @param[in] preserve_mask should the old masks be preserved
		 */
		static inline void relocate_irqs(
				const uint8_t base_isr_number_master,
				const uint8_t base_isr_number_slave,
				const bool preserve_mask = false
				);

		/**
		 * Sends the End Of Interrupt Signal to a specific PIC.
		 * @param[in] irq the involved irq number
		 */
		static inline void send_end_of_interrupt(
				const uint8_t irq);

		/**
		 * Mask or unmasks an interrupt
		 * @param[in] irq_number the interrupt number
		 * @param[in] enable if true this interrupt will be enable (unmasked)
		 * else disabled (masked)
		 */
		static inline void mask_interrupt(
				uint8_t irq_number, bool enable );

	private:
		/**
		 * The port number for PIC Master Commands.
		 */
		static const uint16_t MASTER_COMMAND_PORT	= 0x20;

		/**
		 * The port number for PIC Master Data.
		 */
		static const uint16_t MASTER_DATA_PORT		= 0x21;

		/**
		 * The port number for PIC Slave Commands.
		 */
		static const uint16_t SLAVE_COMMAND_PORT	= 0xA0;

		/**
		 * The port number for PIC Slave Data.
		 */
		static const uint16_t SLAVE_DATA_PORT		= 0xA1;

		/** ICW4 (not) needed */
		static const uint8_t ICW1_ICW4			= 0x01;		
		/** Single (cascade) mode */
		static const uint8_t ICW1_SINGLE		= 0x02;		
		/** Call address interval 4 (8) */
		static const uint8_t ICW1_INTERVAL4		= 0x04;		
		/** Level triggered (edge) mode */
		static const uint8_t ICW1_LEVEL			= 0x08;		
		/** Initialization - required! */
		static const uint8_t ICW1_INIT			= 0x10;		

		/** 8086/88 (MCS-80/85) mode */
		static const uint8_t ICW4_8086			= 0x01;		
		/** Auto (normal) EOI */
		static const uint8_t ICW4_AUTO			= 0x02;		
		/** Buffered mode/slave */
		static const uint8_t ICW4_BUF_SLAVE		= 0x08;		
		/** Buffered mode/master */
		static const uint8_t ICW4_BUF_MASTER	= 0x0C;		
		/** Special fully nested (not) */
		static const uint8_t ICW4_SFNM			= 0x10;		

		/**
		 * The special End Of Interrupt Signal. Clears the Interrupt.
		 */
		static const uint8_t END_OF_INTERRUPT	= 0x20;

};

void pic_8259a::relocate_irqs( 
		const uint8_t base_isr_number_master,
		const uint8_t base_isr_number_slave,
		const bool preserve_mask
		)
{
	uint8_t master_mask = 0;
	uint8_t slave_mask  = 0;

	if (preserve_mask)
	{
		//read ports if we want to preserve flags
		master_mask = io_ports::read_port8( MASTER_DATA_PORT );
		slave_mask  = io_ports::read_port8( SLAVE_DATA_PORT );
	}

	//enter config mode
	io_ports::write_port8( MASTER_COMMAND_PORT, ( ICW1_INIT | ICW1_ICW4 ) );
	io_ports::write_port8( SLAVE_COMMAND_PORT, ( ICW1_INIT | ICW1_ICW4 ) );

	//set base numbers
	io_ports::write_port8( MASTER_DATA_PORT, base_isr_number_master );
	io_ports::write_port8( SLAVE_DATA_PORT, base_isr_number_slave );

	//continue relocation
	io_ports::write_port8( MASTER_DATA_PORT, 0x04 );
	io_ports::write_port8( SLAVE_DATA_PORT, 0x02 );

	//set mode
	io_ports::write_port8( MASTER_DATA_PORT, ICW4_8086 );
	io_ports::write_port8( SLAVE_DATA_PORT, ICW4_8086 );

	//set masks
	io_ports::write_port8( MASTER_DATA_PORT, master_mask );
	io_ports::write_port8( SLAVE_DATA_PORT, slave_mask );
}

void pic_8259a::send_end_of_interrupt(
		const uint8_t irq_number)
{
	if ( irq_number < 16)
	{
		if ( irq_number >= 8) // slave PIC involved?
		{
			// send EOI to slave.
			io_ports::write_port8( SLAVE_COMMAND_PORT, END_OF_INTERRUPT );
		}

		// send EOI to master as the master is always involved
		io_ports::write_port8( MASTER_COMMAND_PORT, END_OF_INTERRUPT);
	}
}

void pic_8259a::mask_interrupt(
	uint8_t irq_number, bool enable )
{
	if ( irq_number >= 16 )
	{
		return;
	}

	uint16_t port( 0 );

	if ( irq_number >= 8 )
	{
		//SLAVE
		port = SLAVE_DATA_PORT;
		irq_number -= 8; // make code sharing possible
	}
	else
	{
		//MASTER
		port = MASTER_DATA_PORT;
	}

	uint8_t mask( 0 );

	switch ( irq_number )
	{
		case 0: mask = 0x01; break;
		case 1: mask = 0x02; break;
		case 2: mask = 0x04; break;
		case 3: mask = 0x08; break;
		case 4: mask = 0x10; break;
		case 5: mask = 0x20; break;
		case 6: mask = 0x40; break;
		case 7: mask = 0x80; break;
		default: return;
	}

	// get the current mask
	uint8_t value( io_ports::read_port8( port ) );

	if ( enable )
	{
		value &= ~mask;
	}
	else
	{
		value |= mask;
	}

	//write modified mask
	io_ports::write_port8( port, value );

}


} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_PIC_8259A_H_ */
