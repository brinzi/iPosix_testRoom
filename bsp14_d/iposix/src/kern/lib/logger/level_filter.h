#ifndef _KERN_LIB_LOGGER_LEVEL_FILTER_H_
#define _KERN_LIB_LOGGER_LEVEL_FILTER_H_

#include "filter.h"

namespace iposix {
namespace logging {

/**
 * A logger filter filtering by the message level.
 */
struct level_filter
	: filter
{
	/**
	 * Constructor.
	 *
	 * @param[in] lvl minimal message level the filter will accept
	 */
	level_filter( message::msg_level lvl )
		: lvl( lvl )
	{
		
	}

	/**
	 * See ::iposix::logging::filter::pass( const message& )
	 */
	inline bool pass( const message& msg ) const
	{
		return msg.level >= this->lvl;
	}

private:
	/**
	 * minimal message level the filter will accept
	 */
	message::msg_level lvl;
};

}} //iposix::logging

#endif /* !_KERN_LIB_LOGGER_LEVEL_FILTER_H_ */
