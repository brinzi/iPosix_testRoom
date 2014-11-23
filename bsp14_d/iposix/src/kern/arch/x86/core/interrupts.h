#ifndef _KERN_ARCH_X86_CORE_INTERRUPTS_H_
#define _KERN_ARCH_X86_CORE_INTERRUPTS_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

/**
 * Descrines generel access to interrupt controlling
 */
class interrupt
{
	public:
		/**
		 * Switches all interrupts off.
		 */
		static void interrupts_off();

		/**
		 * Switches all interrupts on.
		 */
		static void interrupts_on();

		/**
		 * Switches all interrupts on or off.
		 * @param[in] enabled true enables interrupts false disables interrupts
		 */
		static void interrupts( bool enabled );

		/**
		 * Gets the current interrupt state
		 * @return true if interrupts are enabled
		 */
		static bool interrupts();

		/**
		 * Struct that switches interrupts off in current scope like a lock.
		 * On creation the intterupts are switched off and on deletion the
		 * interrupts are switched on.
		 */
		struct scoped_interrupt_off
		{
			public:
				/**
				 * Constructor. Switches interrupts off.
				 * @param[in] preserve_old_value should the old value be saved
				 */
				scoped_interrupt_off( bool preserve_old_value );

				/**
				 * Destructor. Switches interrupts on or restores the old
				 * value.
				 */
				~scoped_interrupt_off();

			private:
				/** The old value */
				bool old_value;
		};

		/**
		 * Struct that switches interrupts on in current scope like a lock.
		 * On creation the intterupts are switched on and on deletion the
		 * interrupts are switched off.
		 */
		struct scoped_interrupt_on
		{
			public:
				/**
				 * Constructor. Switches interrupts on.
				 * @param[in] preserve_old_value should the old value be saved
				 */
				scoped_interrupt_on( bool preserve_old_value );

				/**
				 * Destructor. Switches interrupts off or restores the old
				 * value.
				 */
				~scoped_interrupt_on();

			private:
				/** The old value */
				bool old_value;
		};

	private:

};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_INTERRUPTS_H_ */
