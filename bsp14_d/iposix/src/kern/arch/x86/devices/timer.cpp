#include "x86/devices/timer.h"
#include "x86/io_ports/io_ports.h"
#include "scheduler/i_scheduler.h"

namespace iposix
{
namespace arch
{
namespace x86
{

using ::iposix::scheduler::scheduler_type;

/**
 * For normal timer we use Channel 0, we want to write 2 8bit values (MSB, LSB),
 * we want a square wave mode, and a 16bit counter
 */
const timer::timer_mode timer::TIMER_MODE( 0, 3, 3, 0 );

volatile uint64_t timer::ticks(0);

timer::timer()
{
}

timer::~timer()
{
}

void timer::timer_interrupt_handler( isr_registers& /* unused */ )
{
	++(timer::ticks);

	if ( ( timer::ticks & scheduler_type::ticks_between_reschedulings() ) == 0 )
	{
		scheduler_type::context_switch();
	}
}


void timer::set_frequency(const uint32_t frequency)
{
	uint16_t timer_divisor = INPUT_CLOCK / frequency; 

	// Unfourthunatly it's not possible to send more than 1byte
	// so spilt the divisor
	uint8_t f_lo = static_cast<uint8_t>( timer_divisor & 0xFF );
	uint8_t f_hi = static_cast<uint8_t>( (timer_divisor >> 8) & 0xFF );

	// Set the timer mode
	io_ports::write_port8(TIMER_COMMAND_PORT, TIMER_MODE.bits);
	
	// and now we sent the date
	io_ports::write_port8(TIMER_CHANNEL0_DATA_PORT, f_lo);
	io_ports::write_port8(TIMER_CHANNEL0_DATA_PORT, f_hi);
}

void timer::wait(const uint64_t wait_ticks)
{
	uint64_t wait_until_ticks_reached = (timer::ticks) + wait_ticks;

	//busy waiting
	while (timer::ticks < wait_until_ticks_reached)
	{
		io_ports::delay();
	};
}

} //namespace x86
} //namespace arch
} //namespace iposix
