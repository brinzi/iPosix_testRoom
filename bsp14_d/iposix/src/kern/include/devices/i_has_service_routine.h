#ifndef _KERN_INCLUDE_DEVICES_I_HAS_SERVICE_ROUTINE_H_
#define _KERN_INCLUDE_DEVICES_I_HAS_SERVICE_ROUTINE_H_

#include "core/service_routine.h"

#include <stdint.h>

namespace iposix
{
namespace arch
{

/**
 * Interface for classes that have a service_routine
 */
class i_has_service_routine
{
	public:
		/** Destructor */
		virtual ~i_has_service_routine() {};

		/**
		 * Getter for the service routine
		 * @return the service routine
		 */
		virtual service_routine get_service_routine() = 0;
};

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_DEVICES_I_HAS_SERVICE_ROUTINE_H_ */
