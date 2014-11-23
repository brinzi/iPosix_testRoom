#include <stdint.h>
#include "x86/devices/serial_port.h"
#include "x86/io_ports/io_ports.h"

namespace iposix
{
namespace arch
{
namespace x86
{

serial_port::serial_port()
{
	this->init();
}

serial_port::~serial_port()
{ }

void serial_port::init()
{
	io_ports::write_port8( COM1_PORT_1, PORT_1_DISABLE );
	io_ports::write_port8( COM1_PORT_3, PORT_3_DLAB );   					// Enable DLAB (set baud rate divisor)
	io_ports::write_port8( COM1_PORT_0, BAUD_RATE_DIVISOR & 0xFF );
	io_ports::write_port8( COM1_PORT_1, ( BAUD_RATE_DIVISOR >> 8 ) );
	io_ports::write_port8( COM1_PORT_3, PORT_3_MODE );
	io_ports::write_port8( COM1_PORT_2, PORT_2_MODE );
	io_ports::write_port8( COM1_PORT_4, PORT_4_MODE );
}

void serial_port::real_write( const char c ) const
{
	while ( ( io_ports::read_port8( COM1_PORT_5 ) & SEND_READY ) == 0 );

	io_ports::write_port8( COM1_PORT_0, c );
}

void serial_port::write( const char c ) const
{
	this->real_write( c );

	if ( c == '\n' )
	{
		this->real_write( '\r' );
	}
}

char serial_port::read()
{
	while ( ( io_ports::read_port8( COM1_PORT_5 ) & RECEIVE_READY )  == 0 );
		     
	return io_ports::read_port8( COM1_PORT_0 );
}

} //namespace x86
} //namespace arch
} //namespace iposix
