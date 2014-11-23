#include "x86/core/interrupt_descriptor_table.h"
#include "x86/pic/8259A.h"
#include "logger/logger.h"
#include "syscalls/syscall_dispatcher.h"
#include "exceptions/exceptions.h"
#include "syscalls.h"
#include "die.h"

namespace iposix
{
namespace arch
{
namespace x86
{

using ::iposix::arch::service_routine;
using ::iposix::syscalls::syscall_message;

const service_routine interrupt_descriptor_table::STANDARD_SERVICE_ROUTINE = 
	service_routine( &interrupt_descriptor_table::dummy_dummy_handler, true );

const service_routine interrupt_descriptor_table::SYSCALL_SERVICE_ROUTINE = 
	service_routine( &interrupt_descriptor_table::syscall_handler, true );

const service_routine interrupt_descriptor_table::EXCEPTION_SERVICE_ROUTINE = 
	service_routine( &interrupt_descriptor_table::exception_handler, true );

service_routine interrupt_descriptor_table::irq_handler[IRQs] =
{
	service_routine( &interrupt_descriptor_table::dummy_dummy_handler, false )
};

interrupt_descriptor_table::interrupt_descriptor_table()
:	reg( interrupt_descriptor_table_register( IDT_SIZE, &table[0] ) )
{ }

interrupt_descriptor_table::~interrupt_descriptor_table()
{ }

void interrupt_descriptor_table::init_idt()
{
	//initilize every interrupt with a dummy function
	for (uint_fast16_t i = 0; i < IDT_SIZE; ++i)
	{
		this->set_trap_gate( i, &asm_isr_uninitilized );
	}

	//Exceptions
	this->set_trap_gate(	 0, &asm_isr_0 );
	this->set_trap_gate(	 1, &asm_isr_1 );
	this->set_trap_gate(	 2, &asm_isr_2 );
	this->set_system_gate(	 3, &asm_isr_3 );
	this->set_system_gate(	 4, &asm_isr_4 );
	this->set_system_gate(	 5, &asm_isr_5 );
	this->set_trap_gate(	 6, &asm_isr_6 );
	this->set_trap_gate(	 7, &asm_isr_7 );
	this->set_trap_gate(	 8, &asm_isr_8 );
	this->set_trap_gate(	 9, &asm_isr_9 );
	this->set_trap_gate(	10, &asm_isr_10 );
	this->set_trap_gate(	11, &asm_isr_11 );
	this->set_trap_gate(	12, &asm_isr_12 );
	this->set_trap_gate(	13, &asm_isr_13 );
	this->set_trap_gate(	14, &asm_isr_14 );
	this->set_trap_gate(	15, &asm_isr_15 );
	this->set_trap_gate(	16, &asm_isr_16 );
	this->set_trap_gate(	17, &asm_isr_17 );

	//Reserved Interrupts
	this->set_trap_gate(	18, &asm_isr_18 );
	this->set_trap_gate(	19, &asm_isr_19 );
	this->set_trap_gate(	20, &asm_isr_20 );
	this->set_trap_gate(	21, &asm_isr_21 );
	this->set_trap_gate(	22, &asm_isr_22 );
	this->set_trap_gate(	23, &asm_isr_23 );
	this->set_trap_gate(	24, &asm_isr_24 );
	this->set_trap_gate(	25, &asm_isr_25 );
	this->set_trap_gate(	26, &asm_isr_26 );
	this->set_trap_gate(	27, &asm_isr_27 );
	this->set_trap_gate(	28, &asm_isr_28 );
	this->set_trap_gate(	29, &asm_isr_29 );
	this->set_trap_gate(	30, &asm_isr_30 );
	this->set_trap_gate(	31, &asm_isr_31 );

	//normally irqs are mapped to interrupt 0-15 and the exceptions are mapped
	//to to these interrupts too so we remap them here
	pic_8259a::relocate_irqs( 32, 40 );

	//Hardware interrupts
	this->set_interrupt_gate(	32, &asm_irq_0 );
	this->set_interrupt_gate(	33, &asm_irq_1 );
	this->set_interrupt_gate(	34, &asm_irq_2 );
	this->set_interrupt_gate(	35, &asm_irq_3 );
	this->set_interrupt_gate(	36, &asm_irq_4 );
	this->set_interrupt_gate(	37, &asm_irq_5 );
	this->set_interrupt_gate(	38, &asm_irq_6 );
	this->set_interrupt_gate(	39, &asm_irq_7 );
	this->set_interrupt_gate(	40, &asm_irq_8 );
	this->set_interrupt_gate(	41, &asm_irq_9 );
	this->set_interrupt_gate(	42, &asm_irq_10 );
	this->set_interrupt_gate(	43, &asm_irq_11 );
	this->set_interrupt_gate(	44, &asm_irq_12 );
	this->set_interrupt_gate(	45, &asm_irq_13 );
	this->set_interrupt_gate(	46, &asm_irq_14 );
	this->set_interrupt_gate(	47, &asm_irq_15 );

	// initiazlize the table of irq_handlers
	for ( int i = 0; i < IRQs; ++i )
	{
		this->remove_irq_handler(i);
	}

	//User-Level-Processes Interrupt
	this->set_system_gate(SYSTEM_GATE, &SYSTEM_GATE_ISR);
}

void interrupt_descriptor_table::activate_idt()
{
	//loads the interrupt_descriptor_table_register
	asm volatile("lidt %0" : : "m" (this->reg));
}

void interrupt_descriptor_table::set_interrupt_gate(int entry_number, gate_ptr fct_ptr)
{
	//interrupts musst be handelt in system_privilege_level
	this->table[entry_number] = gate_descriptor(fct_ptr,
			gate_descriptor::interrupt_gate_descriptor, true, privilege::system_privilege_level);
}

void interrupt_descriptor_table::set_system_gate(int entry_number, gate_ptr fct_ptr)
{
	//this gate will be used for system calls
	this->table[entry_number] = gate_descriptor(fct_ptr,
			gate_descriptor::interrupt_gate_descriptor, true, privilege::user_privilege_level);
}

void interrupt_descriptor_table::set_trap_gate(int entry_number, gate_ptr fct_ptr)
{
	//this gate will be used for exceptions and these gates are allowed at every
	//level
	this->table[entry_number] = gate_descriptor(fct_ptr,
			gate_descriptor::trap_gate_descriptor, true, privilege::user_privilege_level);
}

void interrupt_descriptor_table::dispatch_interrupt(isr_registers& regs)
{
	//set a dummy service routine
	service_routine sr = interrupt_descriptor_table::STANDARD_SERVICE_ROUTINE;


	if ( regs.irq_interrupt.number >= 0 ) //hardware irq?
	{
		//Free Interrupt Line
		pic_8259a::send_end_of_interrupt(regs.irq_interrupt.number);

		// Get real Service Routine through the table
		sr = irq_handler[regs.irq_interrupt.number];

		// Disabled?
		if ( ! irq_handler[regs.irq_interrupt.number].enabled )
		{
			logger << message::MORE_VERBOSE
				<< __func__ << ": Hardware Interupt " 
				<< irq_to_string(regs.irq_interrupt.irq).c_str()
				<< "(" << regs.irq_interrupt.number << ")" << " disabled" << endl;
		}
	}
	else if ( regs.exception_interrupt.number == SYSTEM_GATE ) //syscall?
	{
		logger << message::MORE_VERBOSE
			<< __func__ << ": Syscall Interrupt " 
			<< exception_to_string(regs.exception_interrupt.exception).c_str()
			<< "(" << regs.exception_interrupt.number << ")" << endl;

		sr = interrupt_descriptor_table::SYSCALL_SERVICE_ROUTINE;
	}
	else if ( 
			( regs.exception_interrupt.number >= 0 ) && 
			( regs.exception_interrupt.number < EXCEPTIONs )
			) //exception?
	{
		logger << message::MORE_VERBOSE
			<< __func__ << ": Exception Interrupt " 
			<< exception_to_string(regs.exception_interrupt.exception).c_str()
			<< "(" << regs.exception_interrupt.number << ")" << endl;

		sr = interrupt_descriptor_table::EXCEPTION_SERVICE_ROUTINE;
	}
	else //hm...
	{
		logger << message::CRITICAL
			<< __func__ 
			<< "An Interrupt which is whether a hardware interrupt nor an exception nor a syscall occured." 
			<< endl;
	}

	// call Service Routine
	sr.call(regs);

}

void interrupt_descriptor_table::register_irq_handler(
		const uint16_t irq,
		const service_routine h)
{
	if ( irq < IRQs )
	{
		interrupt_descriptor_table::irq_handler[irq] = h;
	}
}

void interrupt_descriptor_table::remove_irq_handler(
		const uint16_t irq)
{
	if ( irq < IRQs )
	{
		interrupt_descriptor_table::irq_handler[irq] = service_routine(
				&interrupt_descriptor_table::dummy_interrupt_handler,
				false);
	}
}

void interrupt_descriptor_table::enable_irq_handler(
		const uint16_t irq)
{
	if ( irq < IRQs )
	{
		interrupt_descriptor_table::irq_handler[irq].enabled = true;
	}
}

void interrupt_descriptor_table::disable_irq_handler(
		const uint16_t irq)
{
	if ( irq < IRQs )
	{
		interrupt_descriptor_table::irq_handler[irq].enabled = false;
	}
}

void interrupt_descriptor_table::set_irq_usability( uint16_t entry_number, bool present )
{
	if ( entry_number < IRQs )
	{
		pic_8259a::mask_interrupt( entry_number, present );
	}
}

void interrupt_descriptor_table::syscall_handler( isr_registers& regs )
{
	syscall_message* in_msg =
		reinterpret_cast<syscall_message*>(regs.eax);

	//TODO musst do something like copy in copy out here since the
	//syscall_message referenced through eax is in userlevel if virtual address
	//management is activated

	::iposix::utils::Singleton< ::iposix::syscalls::syscall_dispatcher >::instance().dispatch_syscall( *in_msg );
}

void interrupt_descriptor_table::exception_handler( isr_registers& regs )
{
	::iposix::syscalls::exception_message msg = 
		::iposix::syscalls::exception_message( 
				exception_to_string( regs.exception_interrupt.exception ),
				regs.exception_interrupt.number,
				regs.error_code,
				regs.eip,
				regs.cr2
				);

	::iposix::utils::Singleton< ::iposix::syscalls::syscall_dispatcher >::instance().dispatch_exception( msg );
}
	
void interrupt_descriptor_table::dummy_interrupt_handler( isr_registers& regs )
{
	logger << message::WARN << "Dummy Interrupt Handler called with "
		<< irq_to_string( regs.irq_interrupt.irq ).c_str()
		<< " Interrupt" << endl;
}

void interrupt_descriptor_table::dummy_dummy_handler( isr_registers& /* unused */ )
{
	logger << message::CRITICAL << "Dummy dummy Handler called!" << endl;

	die();
}

} //namespace x86
} //namespace arch
} //namespace iposix
