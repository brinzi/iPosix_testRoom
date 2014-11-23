#ifndef _KERN_LIB_LOGGER_LOGGER_H_
#define _KERN_LIB_LOGGER_LOGGER_H_

#include "message.h"
#include "filter.h"
#include "level_filter.h"
#include "config/logger.h"
#include "utils/singleton.hpp"


#include <stdio.h>

namespace iposix {
namespace logging {

struct endl_ { };
struct hex_ { };
struct dec_ { };

extern endl_ endl;
extern hex_ hex;
extern dec_ dec;

/**
 * The system logger.
 *
 * @param Video Primary output device (has to implement the functions of a video device)
 * @param Serial Alternative output device (has to implement the functions of a serial device)
 */
template< typename Video, typename Serial >
struct logger
	: virtual public ::iposix::utils::Singleton< logger< Video, Serial > >
{
	friend class ::iposix::utils::Singleton< logger< Video, Serial > >;

	/**
	 * Write a message to the standard and the alternative output.
	 *
	 * @param[in] msg message to write
	 */
	inline void write( const message& msg );

	/**
	 * Sets the standard filter for normal output.
	 *
	 * @param[in] flt filter to use as standard
	 * @return old standard filter
	 */
	inline filter&
	stdfilter( filter& flt )
	{
		filter& old = *this->std_filter;
		this->std_filter = &flt;
		return old;
	}

	/**
	 * Sets the standard filter for alternative output.
	 *
	 * @param[in] flt filter to use as standard
	 * @return old standard filter
	 */
	inline filter&
	altfilter( filter& flt )
	{
		filter& old = *this->alt_filter;
		this->alt_filter = &flt;
		return old;
	}

	/**
	 * Enable use of << operator for printing a c-style string.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] msg c-style string to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( const char * msg )
	{
		this->write( message( msg, this->std_level ) );
		return *this;
	}

	/**
	 * Enable use of << operator for printing a char.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] c char to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( char c )
	{
		char buffer[2];
		buffer[0] = c;
		buffer[1] = 0;

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a unsigned 8 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( uint8_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%hx", val );
		}
		else
		{
			sprintf( &buffer[0], "%hu", val );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a signed 8 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( int8_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%hx", val );
		}
		else
		{
			sprintf( &buffer[0], "%hi", val );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a unsigned 16 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( uint16_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%hx", val );
		}
		else
		{
			sprintf( &buffer[0], "%hu", val );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a signed 16 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( int16_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%hx", val );
		}
		else
		{
			sprintf( &buffer[0], "%hi", val );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a unsigned 32 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( uint32_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%x", static_cast< unsigned int>( val ) );
		}
		else
		{
			sprintf( &buffer[0], "%u", static_cast< unsigned int >( val ) );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a signed 32 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( int32_t val )
	{
		char buffer[15];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%x", static_cast< int >( val ) );
		}
		else
		{
			sprintf( &buffer[0], "%i", static_cast< int >( val ) );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a unsigned 64 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( int64_t val )
	{
		char buffer[30];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%lx", static_cast< long >( val ) );
		}
		else
		{
			sprintf( &buffer[0], "%lu", static_cast< long >( val ) );
		}

		*this << &buffer[0];
		return *this;
	}

	/**
	 * Enable use of << operator for printing a signed 64 bit value.
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] val value to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( uint64_t val )
	{
		char buffer[30];
		if( this->hex )
		{
			sprintf( &buffer[0], "0x%lx", static_cast< unsigned long >( val ) );
		}
		else
		{
			sprintf( &buffer[0], "%lu", static_cast< unsigned long >( val ) );
		}

		*this << &buffer[0];
		return *this;
	}


	/**
	 * Enable use of << operator for printing a ::iposix::logging::message object. 
	 * The standard filters set by stdfilter and altfilter are
	 * used as filters when using this function.
	 *
	 * @param[in] msg message to print
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( const message& msg )
	{
		this->write( msg );
		return *this;
	}

	/**
	 * Enable the use of logger << endl;
	 *
	 * @param[in] endl_& endl_ object.
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( const endl_& )
	{
		*this << "\n";
		return *this;
	}

	/**
	 * Enable the use of logger << hex, to enable hexadecimal output
	 * for integer values.
	 *
	 * @param[in] hex_& hex_ object.
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( const hex_& )
	{
		this->hex = true;
		return *this;
	}

	/**
	 * Enable the use of logger << dec, to disable hexadecimal output
	 * for integer values.
	 *
	 * @param[in] dec_& dec_ object.
	 * @return reference to the logger (to enable sth. like logger << "hello " << "world." << ...)
	 */
	inline logger& operator<< ( const dec_& )
	{
		this->hex = false;
		return *this;
	}

	/**
	 * Part of the variadic template function print, which can be called with
	 * a variable count of parameters for printing, e.g. logger.print( "Hello ", "world", ... )
	 *
	 * @param[in] t object to print
	 */
	template< typename T >
	inline void print( const T& t )
	{
		*this << t;
	}

	/**
	 * Part of the variadic template function print, which can be called with
	 * a variable count of parameters for printing, e.g. logger.print( "Hello ", "world", ... )
	 *
	 * @param[in] t object to print
	 * @param[in] args variable count of objects to print
	 */
	template< typename T, typename... Args >
	inline void print( const T& t, const Args& ... args )
	{
		this->print( t );
		this->print( args ... );
	}

	/**
	 * Sets the standard message level. See ::iposix::logging::message::msg_level.
	 *
	 * @param[in] lvl message level
	 */
	inline void level( message::msg_level lvl = message::VERBOSE )
	{
		this->std_level = lvl;
	}

	/**
	 * Sets the standard message level. See ::iposix::logging::message::msg_level.
	 *
	 * @param[in] lvl message level
	 */
	inline logger& operator << ( const message::msg_level lvl )
	{
		this->level( lvl );
		return *this;
	}

	bool hex;

private:
	/**
	 * Constructor.
	 */
	logger() :
		hex( false ),
		std_filter( 0 ),
		vid( ::iposix::utils::Singleton< Video >::instance() ),
		alt_filter( 0 ),
		serial( ::iposix::utils::Singleton< Serial >::instance() ),
	  	std_level( message::VERBOSE )
	{
		static level_filter ref = level_filter( message::VERBOSE );
		static level_filter alt_ref = level_filter( message::MORE_VERBOSE );

		this->std_filter = &ref;
		this->alt_filter = &alt_ref;
	}

	/**
	 * standard filter (see stdfilter)
	 */
	filter * std_filter;

	/**
	 * Video device to print messages on
	 */
	Video& vid;

	/**
	 * standard filter for alternative output
	 */
	filter * alt_filter;

	/**
	 * Serial device as alternative output for logging messages
	 */
	Serial& serial;

	/**
	 * standard message level (see level( message::msg_level lvl ))
	 */
	message::msg_level std_level;

	/**
	 * Helper function to print a integer value
	 *
	 * @param[in] val integer value
	 * @param[in] format format for the integer value (see sprintf)
	 */
	template< typename T >
	void print_number( T val, const char * format ) 
	{
		char buffer[15];
		sprintf( &buffer[0], &format[0], val );
	}
};

template< typename Video, typename Serial >
void
logger< Video, Serial >::write( const message& msg )
{
	if ( this->alt_filter->pass( msg ) )
	{
		serial.write( &msg.msg[0] );
	}

	if ( this->std_filter->pass( msg ) )
	{
		vid.write( &msg.msg[0] ); 
	}
}

}} //iposix::logging

typedef ::iposix::logging::logger< LOGGER_VIDEO, LOGGER_SERIAL> logger_type;

#define logger ::iposix::utils::Singleton< logger_type >::instance()
//using ::iposix::logging::log;
using ::iposix::logging::message;
using ::iposix::logging::endl;
using ::iposix::logging::hex;
using ::iposix::logging::dec;

#endif /* !_KERN_LIB_LOGGER_LOGGER_H_ */
