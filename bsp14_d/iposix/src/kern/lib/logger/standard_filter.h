#ifndef _KERN_LIB_LOGGER_STANDARD_FILTER_H_
#define _KERN_LIB_LOGGER_STANDARD_FILTER_H_

#include "filter.h"

namespace iposix {
namespace logging {

/**
 * A logger filter which will accept everything
 */
struct standard_filter
	: filter
{
	/**
	 * See ::iposix::logging::filter::pass( const message& )
	 */
	inline bool pass( const message& ) const
	{
		return true;
	}
};

}} //iposix::logging

#endif /* !_KERN_LIB_LOGGER_STANDARD_FILTER_H_ */
