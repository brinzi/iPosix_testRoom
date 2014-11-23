#include "x86/core/interrupts.h"

namespace iposix
{
namespace arch
{
namespace x86
{

void interrupt::interrupts_off()
{
	interrupt::interrupts( false );
}

void interrupt::interrupts_on()
{
	interrupt::interrupts( true );
}

void interrupt::interrupts( bool enabled )
{
	if ( enabled )
	{
		//set interrupts
		asm volatile("sti");
	}
	else
	{
		//clear interrupts
		asm volatile("cli");
	}
}

bool interrupt::interrupts()
{
	uint32_t tmpreg;

	asm volatile(
			"pushf				# push eflags\n\t"
			"pop %0				# pop to parameter\n\t"
			:	"=r" ( tmpreg )
			:	// no inputs
			:	// no clobber list
			);

	return ( ( tmpreg & 0x200 ) == 0x200 );
}

interrupt::scoped_interrupt_off::scoped_interrupt_off( bool preserve_old_value )
	: old_value( true )
{
	if ( preserve_old_value )
	{
		old_value = interrupt::interrupts();
	}

	interrupt::interrupts( false );
}

interrupt::scoped_interrupt_off::~scoped_interrupt_off()
{
	//conditional set on
	interrupt::interrupts( old_value );
}

interrupt::scoped_interrupt_on::scoped_interrupt_on( bool preserve_old_value )
	: old_value( false )
{
	if ( preserve_old_value )
	{
		old_value = interrupt::interrupts();
	}

	interrupt::interrupts( true );
}

interrupt::scoped_interrupt_on::~scoped_interrupt_on()
{
	//conditional set off
	interrupt::interrupts( old_value );
}

} //namespace x86
} //namespace arch
} //namespace iposix
