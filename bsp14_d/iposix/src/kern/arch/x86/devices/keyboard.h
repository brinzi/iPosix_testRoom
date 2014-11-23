#ifndef _KERN_ARCH_X86_DEVICES_KEYBOARD_H_
#define _KERN_ARCH_X86_DEVICES_KEYBOARD_H_

#include "x86/io_ports/io_ports.h"
#include "devices/i_has_service_routine.h"
#include "utils/singleton.hpp"

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Implementation of an common keyboard controller.
 */
class keyboard
	: public i_has_service_routine,
	  public ::iposix::utils::Singleton< keyboard >
{
	friend class ::iposix::utils::Singleton< keyboard >;

	public:
		// i_keyboard

		/**
		 * Gives back the Next Keyboard Scancode
		 * @return the scancode
		 */
		inline uint8_t get_next_event();

		/**
		 * Gives back th Next pressed Key as ASCII.
		 * @return the next pressed key
		 */
		char get_next_key();

		// ~i_keyboard

		// i_has_service_routine

		/**
		 * Returns the Interrupt Service Routine which handles a keyboard
		 * Interupt.
		 * @return the service routine
		 */
		inline service_routine get_service_routine();

		// ~i_has_service_routine

	protected:
		/** Led settings */
		union leds
		{
			/** access to the settings data */
			uint8_t bits;
			/** easy access to the settings */
			struct led_config
			{
				/**
				 * Status of scroll lock led.
				 */
				unsigned scroll_led:1;

				/**
				 * Status of num lock led.
				 */
				unsigned num_led:1;

				/**
				 * Status of caps lock led.
				 */
				unsigned caps_led:1;

				/**
				 * Unused bits.
				 */
				unsigned unused:5;
			} config;

			/**
			 * Constructor
			 * @param[in] scroll_led the status of the scroll lock led
			 * @param[in] num_led the status of the num lock led
			 * @param[in] caps_led the status of the cpas lock led
			 */
			leds(
					bool scroll_led = false, 
					bool num_led = false, 
					bool caps_led = false)
				:	bits(0)
			{
				config.scroll_led = scroll_led ? 1 : 0;
				config.num_led    = num_led    ? 1 : 0;
				config.caps_led   = caps_led   ? 1 : 0;
			};
		};

		/**
		 * The IO Port Number where to read Scancodes from.
		 */
		static const uint16_t KEYBOARD_PORT = 0x60;

		/**
		 * The Scancode for the pressed left shift key.
		 */
		static const uint8_t LEFT_SHIFT_PRESSED = 0x2A;

		/**
		 * The Scancode for the released left shift key.
		 */
		static const uint8_t LEFT_SHIFT_RELEASED = 0xAA;

		/**
		 * The Scancode for the pressed right shift key.
		 */
		static const uint8_t RIGHT_SHIFT_PRESSED = 0x36;

		/**
		 * The Scancode for the released right shift key.
		 */
		static const uint8_t RIGHT_SHIFT_RELEASED = 0xB6;

		/**
		 * The first Part of Scancodes for the Caps Lock Key.
		 * Know that every Caps-Lock press or release results both a press and a
		 * release scancode.
		 */
		static const uint8_t CAPS_LOCK_PART1 = 0x3A;

		/**
		 * The second Part of Scancodes for the Caps Lock Key.
		 * Know that every Caps-Lock press or release results both a press and a
		 * release scancode.
		 */
		static const uint8_t CAPS_LOCK_PART2 = 0xBA;

		/**
		 * The Scancode for the pressed left control key.
		 */
		static const uint8_t LEFT_CTRL_PRESSED = 0x1d;

		/**
		 * The Scancode for the released left control key.
		 */
		static const uint8_t LEFT_CTRL_RELEASED = 0x9D;

		/**
		 * The Scancode for the pressed left alt key.
		 */
		static const uint8_t ALT_PRESSED = 0x38;

		/**
		 * The Scancode for the released left alt key.
		 */
		static const uint8_t ALT_RELEASED = 0xB8;

		/**
		 * The Scancode for an escaped sequence.
		 */
		static const uint8_t ESCAPE = 0xE0;

		/**
		 * The Bit which indicates a key release.
		 */
		static const uint8_t BREAK_BIT = 0x80;

		/**
		 * The Keyboard where to find out whether the keyboard is busy or not.
		 */
		static const uint16_t KEYBOARD_CONTROLLER_BUSY_PORT = 0x64;

		/**
		 * The Keyboard Controller Command which indicates that next data
		 * sets the LEDs
		 */
		static const uint8_t KEYBOARD_LED_SET_FLAG = 0xED;

	private:
		/**
		 * The Size of the Keyboard Buffer.
		 */
		static const uint8_t BUFFER_SIZE = 128;

		/**
		 * The Buffer where to store scancodes.
		 */
		static volatile uint8_t buffer[BUFFER_SIZE];

		/**
		 * The index for the next value in the buffer to write.
		 */
		static volatile uint16_t buffer_in_index;

		/**
		 * The index for the next value in the buffer to read.
		 */
		static volatile uint16_t buffer_out_index;

		/**
		 * indicates whether left shift is activated or not.
		 */
		volatile bool left_shift_state;

		/**
		 * indicates whether right shift is activated or not.
		 */
		volatile bool right_shift_state;

		/**
		 * indicates whether caps lock is activated or not.
		 */
		volatile bool caps_lock_state;

		/**
		 * used for ignoring the second part of the caps lock sequence.
		 */
		volatile bool caps_lock_ignore_one;

		/**
		 * indicates whether super is activated or not. (This key is somewhere
		 * known as the flying windows(r) key.
		 */
		volatile bool super_state;

		/**
		 * indicates whether left control is activated or not.
		 */
		volatile bool left_ctrl_state;

		/**
		 * indicates whether right control is activated or not.
		 */
		volatile bool right_ctrl_state;
		
		/**
		 * indicates whether alternative is activated or not.
		 */
		volatile bool alt_state;

		/**
		 * indicates whether alternative graphic is activated or not.
		 */
		volatile bool alt_gr_state;

		/**
		 * indicates whether a escaped scancode will follow or not.
		 */
		volatile bool escaped;

		/**
		 * Indicates the current states of the leds.
		 */
		static volatile leds led_states;

		/**
		 * Constructor for a common keyboard controller.
		 */
		keyboard();

		/**
		 * Destructor
		 */
		~keyboard();

		/**
		 * Return current state of the num lock LED.
		 */
		inline bool get_num_lock_led_state();

		/**
		 * Return current state of the caps lock LED.
		 */
		inline bool get_caps_lock_led_state();

		/**
		 * Return current state of the scroll lock LED.
		 */
		inline bool get_scroll_lock_led_state();

		/**
		 * Sets the State of the num lock LED.
		 * @param[in] state the new state (true := on)
		 */
		inline void set_num_lock_led_state(const bool state);

		/**
		 * Sets the State of the caps lock LED.
		 * @param[in] state the new state (true := on)
		 */
		inline void set_caps_lock_led_state(const bool state);

		/**
		 * Sets the State of the scroll lock LED.
		 * @param[in] state the new state (true := on)
		 */
		inline void set_scroll_lock_led_state(const bool state);

		/**
		 * Refreshes the State of all LEDs.
		 */
		static inline void refresh_leds();

		/**
		 * Handles an Keyboard Interrupt.
		 * @param[in] regs the register content when the interrupts occours
		 */
		static inline void handle_interrupt( isr_registers& regs);

		/**
		 * Pushes a Scancode to the buffer.
		 * @param[in] event the scancode.
		 */
		static inline void push_event(const uint8_t event);
};

inline bool keyboard::get_num_lock_led_state()
{
	return ( keyboard::led_states.config.num_led == 1 );
}

inline bool keyboard::get_caps_lock_led_state()
{
	return ( keyboard::led_states.config.caps_led == 1 );
}

inline bool keyboard::get_scroll_lock_led_state()
{
	return ( keyboard::led_states.config.scroll_led == 1 );
}

inline void keyboard::set_num_lock_led_state(const bool state)
{
	keyboard::led_states.config.num_led = state ? 1 : 0;
	this->refresh_leds();
}

inline void keyboard::set_caps_lock_led_state(const bool state)
{
	keyboard::led_states.config.caps_led = state ? 1 : 0;
	this->refresh_leds();
}

inline void keyboard::set_scroll_lock_led_state(const bool state)
{
	keyboard::led_states.config.scroll_led = state ? 1 : 0;
	this->refresh_leds();
}

inline void keyboard::refresh_leds()
{
	// wait until ready
	while( (io_ports::read_port8( KEYBOARD_CONTROLLER_BUSY_PORT ) & 2 ) != 0 )
	{
	}

	io_ports::write_port8( KEYBOARD_PORT , KEYBOARD_LED_SET_FLAG );

	// wait until ready
	while( (io_ports::read_port8( KEYBOARD_CONTROLLER_BUSY_PORT ) & 2 ) != 0 )
	{
	}

	io_ports::write_port8( KEYBOARD_PORT , keyboard::led_states.bits );
}

void keyboard::handle_interrupt( isr_registers& /* unused */ )
{
	uint8_t scan_code = io_ports::read_port8(KEYBOARD_PORT);

	keyboard::push_event(scan_code);
}

void keyboard::push_event(uint8_t event)
{
	if ( ! 
			(
			 (keyboard::buffer_in_index == ( keyboard::buffer_out_index - 1 ) ) ||
			 ( 
			  ( keyboard::buffer_in_index == keyboard::BUFFER_SIZE - 1 ) && 
			  ( keyboard::buffer_out_index == 0 ) 
			 )
			)
	   )
	{ 
		keyboard::buffer[keyboard::buffer_in_index++] = event;
		if (keyboard::buffer_in_index >= keyboard::BUFFER_SIZE)
		{
			keyboard::buffer_in_index = 0;
		}
	}
}

uint8_t keyboard::get_next_event()
{
	uint8_t ret = 0;

	if (keyboard::buffer_in_index != keyboard::buffer_out_index)
	{
		ret = keyboard::buffer[keyboard::buffer_out_index++];
		if (keyboard::buffer_out_index >= keyboard::BUFFER_SIZE)
		{
			keyboard::buffer_out_index = 0;
		}
	}

	return ret;
}

service_routine keyboard::get_service_routine()
{
	return service_routine( &keyboard::handle_interrupt );
}

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_DEVICES_KEYBOARD_H_ */
