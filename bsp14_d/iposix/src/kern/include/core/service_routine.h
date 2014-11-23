#ifndef _KERN_INCLUDE_CORE_SERVICE_ROUTINE_H_
#define _KERN_INCLUDE_CORE_SERVICE_ROUTINE_H_

#include "config/isr_registers.h"
#include <stdint.h>

namespace iposix
{
namespace arch
{

/**
 * A static function which handles the interrupt and gets
 * the content of some registers.
 */
typedef void (*service_routine_ptr)( isr_registers& regs );

/**
 * Desribes a service routine which can handle a spezific interrupt.
 */
struct service_routine
{
	public:
		/**
		 * Constructs a null service_routine - do not call this.
		 */
		service_routine()
		:	enabled( false ),
			fct_ptr( 0)
		{
		};

		/**
		 * Constructs a interrupt service routine.
		 * @param[in] fct_ptr the function which knows how to handle the interrupt
		 * @param[in] enabled should the depending interrupt be handled
		 */
		service_routine( service_routine_ptr fct_ptr, bool enabled = true )
		:	enabled( enabled ),
			fct_ptr( fct_ptr )
		{
		};

		/**
		 * Should the dispatcher call this routine
		 */
		bool enabled;

		/**
		 * Calls the depending Service Routine.
		 * @param[in] regs the Register Content
		 */
		inline void call( isr_registers& regs )
		{
			if ( enabled )
			{
				fct_ptr( regs );
			}
		}

	private:
		/**
		 * Static Handler function which can call functions in object.
		 */
		service_routine_ptr fct_ptr;

};

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_CORE_SERVICE_ROUTINE_H_ */
