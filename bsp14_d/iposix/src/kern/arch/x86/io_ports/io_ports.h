#ifndef _KERN_ARCH_X86_IO_PORTS_IO_PORTS_H_
#define _KERN_ARCH_X86_IO_PORTS_IO_PORTS_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

class io_ports
{
	public:
		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] value the value to write
		 */
		static inline void write_port8( const uint16_t port, const uint8_t value );

		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] value the value to write
		 */
		static inline void write_port16( const uint16_t port, const uint16_t value );

		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] value the value to write
		 */
		static inline void write_port32( const uint16_t port, const uint32_t value );

		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] addr the address of the string to write
		 * @param[in] length the length of the string to write
		 */
		static inline void write_string8( 
				const uint16_t port, 
				const void* addr, 
				uint32_t length );

		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] addr the address of the string to write
		 * @param[in] length the length of the string to write
		 */
		static inline void write_string16( 
				const uint16_t port, 
				const void* addr, 
				uint32_t length );

		/**
		 * Writes directly to a port
		 * @param[in] port the port to write to
		 * @param[in] addr the address of the string to write
		 * @param[in] length the length of the string to write
		 */
		static inline void write_string32( 
				const uint16_t port, 
				const void* addr, 
				uint32_t length );

		/**
		 * Reads a value from a given port.
		 * @param[in] port the port to read from
		 * @return the value which was read
		 */
		static inline uint8_t read_port8( const uint16_t port );

		/**
		 * Reads a value from a given port.
		 * @param[in] port the port to read from
		 * @return the value which was read
		 */
		static inline uint16_t read_port16( const uint16_t port );

		/**
		 * Reads a value from a given port.
		 * @param[in] port the port to read from
		 * @return the value which was read
		 */
		static inline uint32_t read_port32( const uint16_t port );

		/**
		 * Reads values from a given port.
		 * @param[in] port the port to read from
		 * @param[in] addr the address of the destination string
		 * @param[in] length the length of the string
		 */
		static inline void read_string8( const uint16_t port, void* addr, uint32_t length);

		/**
		 * Reads values from a given port.
		 * @param[in] port the port to read from
		 * @param[in] addr the address of the destination string
		 * @param[in] length the length of the string
		 */
		static inline void read_string16( const uint16_t port, void* addr, uint32_t length);

		/**
		 * Reads values from a given port.
		 * @param[in] port the port to read from
		 * @param[in] addr the address of the destination string
		 * @param[in] length the length of the string
		 */
		static inline void read_string32( const uint16_t port, void* addr, uint32_t length);

		/**
		 * Delays the programmflow for almost (count * 1)microseconds.
		 * @param[in] count the number of microsecods
		 */
		static inline void delay(const uint32_t count = 1);

	private:
};

void io_ports::write_port8( const uint16_t port, const uint8_t value )
{
	//out single byte
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void io_ports::write_port16( const uint16_t port, const uint16_t value )
{
	//out single word (2xbyte)
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

void io_ports::write_port32( const uint16_t port, const uint32_t value )
{
	//out single doubleword (4xbyte)
	asm volatile ("outl %1, %0" : : "dN" (port), "a" (value));
}

void io_ports::write_string8( const uint16_t port, const void* addr, uint32_t length )
{
	//out single byte
	asm volatile ("rep outsb" : "+S"(addr), "+c"(length) : "d" (port));
}

void io_ports::write_string16( const uint16_t port, const void* addr, uint32_t length )
{
	//out single word (2xbyte)
	asm volatile ("rep outsw" : "+S"(addr), "+c"(length) : "d" (port));
}

void io_ports::write_string32( const uint16_t port, const void* addr, uint32_t length )
{
	//out single doubleword (4xbyte)
	asm volatile ("rep outsl" : "+S"(addr), "+c"(length) : "d" (port));
}

uint8_t io_ports::read_port8( const uint16_t port )
{
	uint8_t read_value;

	//in single byte
	asm volatile ("inb %1, %0" : "=a" (read_value) : "dN" (port));

	return read_value;
}

uint16_t io_ports::read_port16( const uint16_t port )
{
	uint16_t read_value;

	//in single word (2xbyte)
	asm volatile ("inw %1, %0" : "=a" (read_value) : "dN" (port));

	return read_value;
}

uint32_t io_ports::read_port32( const uint16_t port )
{
	uint32_t read_value;

	//in single doubleword (4xbyte)
	asm volatile ("inl %1, %0" : "=a" (read_value) : "dN" (port));

	return read_value;
}

void io_ports::read_string8( const uint16_t port, void* addr, uint32_t length)
{
	asm volatile ( "rep insb" : "+D"( addr ), "+c"( length ) : "d"( port ) );
}

void io_ports::read_string16( const uint16_t port, void* addr, uint32_t length)
{
	asm volatile ( "rep insw" : "+D"( addr ), "+c"( length ) : "d"( port ) );
}

void io_ports::read_string32( const uint16_t port, void* addr, uint32_t length)
{
	asm volatile ( "rep insl" : "+D"( addr ), "+c"( length ) : "d"( port ) );
}

void io_ports::delay(const uint32_t count)
{
	for (uint32_t i = 0; i < count; ++i)
	{
		asm volatile( "pause" );	// like nop - instruction used for slowing down spin-wait loops
									// see
									// http://www.intel.com/cd/ids/developer/emea/rus/19139.htm?page=2
		io_ports::read_port8(0x80);
	}
}

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_IO_PORTS_IO_PORTS_H_ */
