#ifndef _KERN_LIB_LOGGER_FILTER_H_
#define _KERN_LIB_LOGGER_FILTER_H_

#include "message.h"

namespace iposix {
namespace logging {

/**
 * Interface for a logger filter
 */
struct filter
{
	/**
	 * Destructor.
	 */
	virtual ~filter() {};

	/**
	 * This function has to return true if the message
	 * shall pass, false otherwise.
	 *
	 * @param[in] msg message to check
	 * @return true if message shall pass
	 */
	virtual bool pass( const message& msg ) const = 0;
};

}} //iposix::logging

#endif /* !_KERN_LIB_LOGGER_FILTER_H_ */
