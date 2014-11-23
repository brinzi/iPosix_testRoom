#ifndef _KERN_INCLUDE_DEVICES_I_VIDEO_H_
#define _KERN_INCLUDE_DEVICES_I_VIDEO_H_

#include <stdint.h>
#include <stdio.h>
#include "utils/singleton.hpp"

namespace iposix {
namespace arch {

/**
 * Hardware independent part of a video device
 *
 * @param Video Hardware dependent part of a video device
 */
template< typename Video >
class i_video
	: public Video,
	  public ::iposix::utils::Singleton< i_video< Video > >
{
	friend class ::iposix::utils::Singleton< i_video< Video > >;

	public:
		/**
		 * Method for writing one char to terminal.
		 *
		 * @param[in] c char
		 */
		void write( const char c );

		/**
		 * Method for writing multiple chars to terminal
		 * @param[in] c_ptr Pointer to Char-Array
		 */
		void write( const char* c_ptr );

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		void write( int32_t i, bool hex = true );

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		void write( int64_t i, bool hex = true );

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		void write( uint32_t i, bool hex = true );

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		inline void write( uint8_t i, bool hex = true )
		{
			write( static_cast<uint32_t>(i), hex );
		}

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		inline void write( uint16_t i, bool hex = true )
		{
			write( static_cast<uint32_t>(i), hex );
		}

		/**
		 * Method for writing an integer to terminal.
		 * @param[in] i the integer to write
		 * @param[in] hex hexadecimal output
		 */
		void write( uint64_t i, bool hex = true );

	private:
		/**
		 * Constructor.
		 */
		i_video()
			: Video()
		{
		}
};

template< typename Video >
void i_video< Video >::write( const char c )
{
	Video::write(c);
}

template< typename Video >
void i_video< Video >::write( const char* c_ptr )
{
	for (const char* c = c_ptr; *c; ++c)
	{
		Video::write(*c);
	}
}

template< typename Video >
void i_video< Video >::write( int32_t n, bool hex )
{
	char buffer[30];
	if (hex)
	{
		sprintf( &buffer[0], "%x", static_cast<unsigned int>(n) );
	}
	else
	{
		sprintf( &buffer[0], "%d", static_cast<int>(n) );
	}
	this->write( &buffer[0] );
}

template< typename Video >
void i_video< Video >::write( uint32_t n, bool hex )
{
	char buffer[30];
	if (hex)
	{
		sprintf( &buffer[0], "%x", static_cast<unsigned int>(n) );
	}
	else
	{
		sprintf( &buffer[0], "%d", static_cast<int>(n) );
	}
	this->write( &buffer[0] );
}

template< typename Video >
void i_video< Video >::write( int64_t n, bool hex )
{
	char buffer[30];
	if (hex)
	{
		sprintf( &buffer[0], "%lx", static_cast<long unsigned int>( n ) );
	}
	else
	{
		sprintf( &buffer[0], "%ld", static_cast<long int>(n) );
	}
	this->write( &buffer[0] );
}

template< typename Video >
void i_video< Video >::write( uint64_t n, bool hex )
{
	char buffer[30];
	if (hex)
	{
		sprintf( &buffer[0], "%lx", static_cast<long unsigned int>(n) );
	}
	else
	{
		sprintf( &buffer[0], "%ld", static_cast<long int>(n) );
	}
	this->write( &buffer[0] );
}

}} //iposix::arch

#endif /* !_KERN_INCLUDE_DEVICES_I_VIDEO_H_ */
