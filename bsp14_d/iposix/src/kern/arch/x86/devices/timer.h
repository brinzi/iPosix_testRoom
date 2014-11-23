#ifndef _KERN_ARCH_X86_DEVICES_TIMER_H_
#define _KERN_ARCH_X86_DEVICES_TIMER_H_

#include <stdint.h>
#include "devices/i_has_service_routine.h"
#include "utils/singleton.hpp"

namespace iposix
{
namespace arch
{

/**
 * Includes a timer
 */
namespace x86
{

/**
 * A common IRQ based timer controller.
 */
class timer 
	: public i_has_service_routine,
	  public ::iposix::utils::Singleton< timer >
{
	friend class ::iposix::utils::Singleton< timer >;

	public:
		/**
		 * Set the frequency in which the timer generates interrupts.
		 * @param[in] frequency the frequency (in Hz)
		 */
		static void set_frequency( const uint32_t frequency );

		// i_timer
		
		/**
		 * Wait for n Ticks. Don't use this unless timer is set up.
		 * @param[in] ticks the amount of ticks to wait for
		 */
		void wait( const uint64_t ticks );

		/**
		 * Returns the current tick counter
		 * @return the counter
		 */
		inline uint64_t get_ticks();

		// ~i_timer

		// i_has_service_routine

		/**
		 * Returns back the Interrupt Service Routine which handles a timer
		 * interrupt.
		 * @return the service routine
		 */
		inline service_routine get_service_routine();	

		// ~i_has_service_routine

	protected:
		/** Describes the settings for the timer  */
		union timer_mode
		{
			/** pure access to the data */
			uint8_t bits;
			/** easy acces to the settings */
			struct timer_config
			{
                /**
				 * Counter
				 * 0 := 16bit counter
				 * 1 := 4x Binary Code Dezimal
                 */
				unsigned counter_type:1;

                /**
				 * Mode
				 * 0 := Interrupt on terminal count
				 * 1 := Hardware Retriggerable one shot
				 * 2 := Rate Generator
				 * 3 := Square Wave Mode
				 * 4 := Software Strobe
				 * 5 := Hardware Strobe
                 */
				unsigned mode:3;

                /**
				 * Read write Mode
				 * 1 := LSB
				 * 2 := MSB
				 * 3 := LSB then MSB
                 */
				unsigned rw_mode:2;

                /**
				 * Counter number (0 - 2)
                 */
				unsigned counter_nr:2;

			} config;

			/**
			 * Constructor
			 * @param[in] counter_nr the number of the counter see counter_type
			 * @param[in] rw_mode the read write mode of the timer see rw_mode
			 * @param[in] mode the mode of the time see mode
			 * @param[in] counter_type the type of the counter
			 */
			timer_mode(
					uint8_t counter_nr = 0,
					uint8_t rw_mode = 3,
					uint8_t mode = 3,
					uint8_t counter_type = 0)
			:	bits(0)
			{
				config.counter_nr = counter_nr;
				config.rw_mode = rw_mode;
				config.mode = mode;
				config.counter_type = counter_type;
			};
		};

		/**
		 * Creates an IRQ based Timer
		 */
		timer();

		/**
		 * Destruktor
		 */
		~timer();

		/**
		 * The Hardware Clock Constant.
		 */
		static const uint32_t INPUT_CLOCK = 1193180;

		/**
		 * Port for loading timer0
		 */
		static const uint16_t TIMER_CHANNEL0_DATA_PORT = 0x40;

		/**
		 * Port for loading timer1
		 */
		static const uint16_t TIMER_CHANNEL1_DATA_PORT = 0x41;

		/**
		 * Port for loading timer2
		 */
		static const uint16_t TIMER_CHANNEL2_DATA_PORT = 0x42;

		/**
		 * The Port to configure the timer.
		 */
		static const uint16_t TIMER_COMMAND_PORT = 0x43;

	private:
		/**
		 * Standard Timer Configuration.
		 */
		static const timer_mode TIMER_MODE;

		/**
		 * Service Routines counted ticks.
		 */
		static volatile uint64_t ticks;

		/**
		 * Handles an IRQ_Timer Interrupt.
		 * @param[in] regs the Register Content
		 */
		static void timer_interrupt_handler( isr_registers& regs );
};

uint64_t timer::get_ticks()
{
	return timer::ticks;
}

service_routine timer::get_service_routine()
{
	return service_routine( &timer::timer_interrupt_handler );
}

} //namespace x86
} //namespace arch
} //namespace iposix


#endif /* !_KERN_ARCH_X86_DEVICES_TIMER_H_ */
