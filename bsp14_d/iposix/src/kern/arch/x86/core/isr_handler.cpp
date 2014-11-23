#include "x86/core/interrupt_descriptor_table.h"

extern "C" void isr_handler(iposix::arch::x86::isr_registers regs)
{
	iposix::arch::x86::interrupt_descriptor_table::dispatch_interrupt(regs);
}

