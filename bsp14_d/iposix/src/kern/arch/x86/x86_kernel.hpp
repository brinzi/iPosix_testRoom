#ifndef _KERN_ARCH_X86_X86_KERNEL_HPP_
#define _KERN_ARCH_X86_X86_KERNEL_HPP_

#include <stdint.h>
#include "scheduler/i_process.h"
#include "devices/i_core_holder.h"
#include "x86/core/interrupts.h"
#include "utils/singleton.hpp"
#include "logger/logger.h"

namespace iposix
{
namespace arch
{
namespace x86
{

using ::iposix::arch::core_type;
using ::iposix::arch::core_holder_type;
using ::iposix::scheduler::process_type;

/**
 * Desribes the common x86 architecture.
 */
template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
class x86_kernel
{
	public:
		/**
		 * Constructor for Hardware Depending Stuff.
		 */
		x86_kernel();

		/**
		 * Destructor for Hardware Depending Stuff.
		 */
		~x86_kernel();

		// i_arch

		/**
		 * Initialisation of Architekture spezific Stuff. Memory, Interrupts.
		 */
		void set_up_architecture();

		/**
		 * Set the initial process
		 * @param[in] process the initial process
		 */
		bool set_init_process( process_type& process );

		/**
		 * Switches the CPU to User Mode.
		 */
		inline void switch_to_user_mode();

		/**
		 * Prepares the Architekture for a Shutdown.
		 */
		void set_down_architecture();

		// ~i_arch

		Video& video;

		Keyboard& keyboard;

		Timer& timer;

		Sound& sound;

		Serial& serial_port;

		Memory& memory;

		Ata& ata;

	private:
		inline core_type& current_core();
};

template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
x86_kernel< Timer, Video, Keyboard, Sound, Serial, Memory, Ata >::x86_kernel()
:	video( ::iposix::utils::Singleton< Video >::instance() ),
	keyboard( ::iposix::utils::Singleton< Keyboard >::instance() ),
	timer( ::iposix::utils::Singleton< Timer >::instance() ),
	sound( ::iposix::utils::Singleton< Sound >::instance() ),
	serial_port( ::iposix::utils::Singleton< Serial >::instance() ),
	memory( ::iposix::utils::Singleton< Memory >::instance() ),
	ata( ::iposix::utils::Singleton< Ata >::instance() )
{
}

template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
x86_kernel< Timer, Video, Keyboard, Sound, Serial, Memory, Ata >::~x86_kernel()
{
}

template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
core_type& x86_kernel< Timer, Video, Keyboard, Sound, Serial, Memory, Ata >::current_core()
{
	return ::iposix::utils::Singleton< core_holder_type >::instance().current_core();
}

template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
void x86_kernel< Timer, Video, Keyboard, Sound, Serial, Memory, Ata >::set_up_architecture()
{
	logger << message::VERBOSE << "Setting up Serial Port... " << endl;
	this->serial_port.init();

	logger << message::VERBOSE << "Setting up Timer... " << endl;
	this->timer.set_frequency(100);

	logger << message::VERBOSE << "Setting up Interrupt Handlers... " << endl;
	this->current_core().register_irq_handler(0, this->timer.get_service_routine());

	this->current_core().register_irq_handler(1, this->keyboard.get_service_routine());

	//deactivate (mask) all other irqs
	//this->current_core().set_irq_usability( 0, false );
	//this->current_core().set_irq_usability( 1, false );
	this->current_core().set_irq_usability( 2, false );
	this->current_core().set_irq_usability( 3, false );
	this->current_core().set_irq_usability( 4, false );
	this->current_core().set_irq_usability( 5, false );
	this->current_core().set_irq_usability( 6, false );
	this->current_core().set_irq_usability( 7, false );
	this->current_core().set_irq_usability( 8, false );
	this->current_core().set_irq_usability( 9, false );
	this->current_core().set_irq_usability( 10, false );
	this->current_core().set_irq_usability( 11, false );
	this->current_core().set_irq_usability( 12, false );
	this->current_core().set_irq_usability( 13, false );
	this->current_core().set_irq_usability( 14, false );
	this->current_core().set_irq_usability( 15, false );

	logger << message::VERBOSE << "Setting up ATA Drives... " << endl;
	this->ata.init();
}

template< typename Timer, typename Video, typename Keyboard, typename Sound, typename Serial, typename Memory, typename Ata >
void x86_kernel< Timer, Video, Keyboard, Sound, Serial, Memory, Ata >::set_down_architecture()
{
}

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_X86_KERNEL_HPP_ */
