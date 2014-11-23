#include <stdint.h>
#include "x86/devices/sound.h"
#include "x86/io_ports/io_ports.h"

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * For sound timer we use Channel 2, we want to write 2 8bit values (MSB, LSB),
 * we want a square wave mode, and a 16bit counter
 */
const timer::timer_mode sound::SOUND_TIMER_MODE( 2, 3, 3, 0 );

sound::sound()
{
}

sound::~sound()
{
}

void sound::beep()
{
	 play_sound(500);
	 wait(10);
	 stop_sound();
}

void sound::play_sound( const uint32_t frequency )
{
	uint32_t sound_divisor = INPUT_CLOCK / frequency;
	uint8_t tmp = 0;

	// Unfourthunatly it's not possible to send more than 1byte
	// so spilt the divisor
	uint8_t f_lo = static_cast<uint8_t>( sound_divisor & 0xFF );
	uint8_t f_hi = static_cast<uint8_t>( (sound_divisor >> 8) & 0xFF );

	// Set the timer mode
	io_ports::write_port8( TIMER_COMMAND_PORT, SOUND_TIMER_MODE.bits );

	// and now we sent the date
	io_ports::write_port8( TIMER_CHANNEL2_DATA_PORT, f_lo );
	io_ports::write_port8( TIMER_CHANNEL2_DATA_PORT, f_hi );

	//And play the sound using the PC speaker
	tmp = io_ports::read_port8( SOUND_PORT );
 	if ( tmp != ( tmp | SOUND_PLAY_MASK ) )
	{
		io_ports::write_port8( SOUND_PORT, ( tmp | SOUND_PLAY_MASK ) );
	}
}

void sound::stop_sound()
{
	uint8_t tmp = io_ports::read_port8( SOUND_PORT );
    
	if ( tmp != ( tmp & SOUND_STOP_MASK ) )
	{
		io_ports::write_port8( SOUND_PORT, ( tmp & SOUND_STOP_MASK ) );
	}
}

} //namespace x86
} //namespace arch
} //namespace iposix
