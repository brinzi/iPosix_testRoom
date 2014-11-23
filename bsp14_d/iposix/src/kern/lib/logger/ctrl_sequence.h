#ifndef _KERN_LIB_LOGGER_CTRL_SEQUENCE_H_
#define _KERN_LIB_LOGGER_CTRL_SEQUENCE_H_

namespace iposix {
namespace logging {

struct logger;

struct control_sequence
{
	virtual ~control_sequence() {};

	virtual void exec( logger& ) const = 0;
};

}} //iposix::logging

#endif /* !_KERN_LIB_LOGGER_CTRL_SEQUENCE_H_ */
