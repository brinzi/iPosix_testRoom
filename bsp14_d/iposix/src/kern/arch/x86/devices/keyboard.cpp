#include <stdint.h>
#include "x86/devices/keyboard.h"
#include "x86/devices/keyboard_at_scancodes.h"
#include "x86/io_ports/io_ports.h"

namespace iposix
{
namespace arch
{
namespace x86
{

volatile uint8_t keyboard::buffer[BUFFER_SIZE];

volatile uint16_t keyboard::buffer_in_index(0);

volatile uint16_t keyboard::buffer_out_index(0);

volatile keyboard::leds keyboard::led_states = leds();

keyboard::keyboard()
:	left_shift_state( false ),
	right_shift_state( false ),
	caps_lock_state( false ),
	caps_lock_ignore_one( false ),
	super_state( false ),
	left_ctrl_state( false ),
	right_ctrl_state( false ),
	alt_state( false ),
	alt_gr_state( false ),
	escaped( false )
{
	for (int i = 0; i < BUFFER_SIZE; ++i)
	{
		this->buffer[i] = 0;
	}
}

keyboard::~keyboard()
{
}

char keyboard::get_next_key()
{
	uint8_t scancode = keyboard::get_next_event();
	uint8_t ret = 0;

	switch (scancode)
	{
		case CAPS_LOCK_PART2:
			break;

		case CAPS_LOCK_PART1:
			if (this->escaped)
			{
				if (!this->caps_lock_ignore_one)
				{
					this->caps_lock_state = !this->caps_lock_state;
				}
				this->caps_lock_ignore_one = !this->caps_lock_ignore_one;
				this->escaped = false;
			}
			else
			{
				if (!this->caps_lock_ignore_one)
				{
					this->caps_lock_state = !this->caps_lock_state;
				}
				this->caps_lock_ignore_one = !this->caps_lock_ignore_one;
			}
			break;
			
		case LEFT_SHIFT_PRESSED:
			if (this->escaped)
			{
				this->left_shift_state = true;
				this->escaped = false;
			}
			else
			{
				this->left_shift_state = true;
			}
			break;

		case LEFT_SHIFT_RELEASED:
			if (this->escaped)
			{
				this->left_shift_state = false;
				this->escaped = false;
			}
			else
			{
				this->left_shift_state = false;
			}
			break;

		case RIGHT_SHIFT_PRESSED:
			if (this->escaped)
			{
				this->right_shift_state = true;
				this->escaped = false;
			}
			else
			{
				this->right_shift_state = true;
			}
			break;

		case RIGHT_SHIFT_RELEASED:
			if (this->escaped)
			{
				this->right_shift_state = false;
				this->escaped = false;
			}
			else
			{
				this->right_shift_state = false;
			}
			break;

		case LEFT_CTRL_PRESSED:
			if (this->escaped)
			{
				this->left_ctrl_state = true;
				this->escaped = false;
			}
			else
			{
				this->left_ctrl_state = true;
			}
			break;
	
		case LEFT_CTRL_RELEASED:
			if (this->escaped)
			{
				this->left_ctrl_state = false;
				this->escaped = false;
			}
			else
			{
				this->left_ctrl_state = false;
			}
			break;

		case ALT_PRESSED:
			if (this->escaped)
			{
				this->alt_gr_state = true;
				this->escaped = false;
			}
			else
			{
				this->alt_state = true;
			}
			break;

		case ALT_RELEASED:
			if (this->escaped)
			{
				this->alt_gr_state = false;
				this->escaped = false;
			}
			else
			{
				this->alt_state = false;
			}
			break;

		case ESCAPE:
			this->escaped = true;

		default:
			if ( ! (scancode & BREAK_BIT) )
			{
				if (scancode > 128)
				{
					//ERROR
					ret = 0;
				}
				else if (this->escaped)
				{
					ret = keyboard_german[scancode][3];
					this->escaped = false;
				}
				else if (this->alt_gr_state)
				{
					ret = keyboard_german[scancode][2];
				}
				else if (this->left_shift_state || this->right_shift_state || this->caps_lock_state)
				{
					ret = keyboard_german[scancode][1];
				}
				else
				{
					ret = keyboard_german[scancode][0];
				}

                /*
				 *if (ret == 0)
				 *{
				 *    ret = scancode;
				 *}
                 */
			}
			break;
	}

	return ret;
}

} //namespace x86
} //namespace arch
} //namespace iposix
