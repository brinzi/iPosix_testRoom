#ifndef _KERN_LIB_LOGGER_MESSAGE_H_
#define _KERN_LIB_LOGGER_MESSAGE_H_

#include "identifiable.h"
#include <stdint.h>

#define MSG_SIZE 1000

namespace iposix {
namespace logging {

/**
 * Message object for the logger
 */
struct message
{
	/**
	 * Describes whether message is for stdout or stderr. Not used in iposix yet.
	 */
	enum type
	{
		STDOUT,
		STDERR
	};

	/**
	 * The message level. Mainly relevant when the level filter is used by the logger.
	 */
	enum msg_level
	{
		TOO_VERBOSE = 0,	// for example interrupt service routines
		MORE_VERBOSE = 10,	// thing which should only printed through alt_video
		VERBOSE = 20,		// maybe every thing we OS does
		INFO = 30,			// Important Informations
		WARN = 40,			// Warnings
		ERROR = 50,			// Normal Erros
		CRITICAL = 60,		// Errors which lets to a Kernel Stop
		HIGHEST = 60		// Just for debug purpose
	};
	
	/**
	 * Sender of the message.
	 */
	identifiable * sender;
	/**
	 * Type of the message.
	 */
	type message_type;
	/**
	 * The message itself.
	 */
	char msg[MSG_SIZE];
	/**
	 * error occured?
	 */
	bool error;
	/**
	 * message level
	 */
	msg_level level;

	/**
	 * Explicit constructor.
	 *
	 * @param[in] message the message in c-style string
	 * @param[in] level the message level
	 * @param[in] message_type the message type
	 * @param[in] sender an optional pointer to the sender of the message
	 */
	explicit message( const char * message = 0, msg_level level = VERBOSE, type message_type = STDOUT, identifiable * sender = 0 );

private:
	/**
	 * helper variable for copying messages
	 */
	uint32_t position;
};

}} //iposix::logging

#endif /* !_KERN_LIB_LOGGER_MESSAGE_H_ */
