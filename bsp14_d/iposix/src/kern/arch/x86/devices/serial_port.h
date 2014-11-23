#ifndef _KERN_ARCH_X86_DEVICES_SERIAL_PORT_H_
#define _KERN_ARCH_X86_DEVICES_SERIAL_PORT_H_

#include <stdint.h>
#include "devices/i_video.h"
#include "utils/noncopyable.h"

namespace iposix {
namespace arch {
namespace x86 {

enum baud_rate_divisors
{
	baud_115200 = 1,
	baud_57600 = 2,
	baud_38400 = 3,
	baud_19200 = 6,
	baud_9600 = 12,
	baud_4800 = 24,
	baud_2400 = 48,
	baud_600 = 192,
	baud_300 = 384,
	baud_50 = 2304
};

/**
 * Desribes a Serial Port on COM1.
 * MORE INFORMATION: http://www.beyondlogic.org/serial/serial.htm
 */
class serial_port
	: virtual public ::iposix::utils::Noncopyable
{
	friend class ::iposix::arch::i_video< serial_port >; //only i_video can use this

	public:
		/**
		 * Initializes a Serial Port
		 */
		static void init();

		/**
		 * Reads a char from the Serial Port
		 * @return the char
		 */
		char read();

		// i_video

		/**
		 * Does nothing since it is not possible to clear sended date on a
		 * serial line (Or am I wrong? But it defenitly does nothing)
		 */
		inline void clear() const { };

		/**
		 * Writes a char to the serial port.
		 */
		void write(const char c) const;

		// ~i_video

	protected:
		static const uint16_t COM1_PORT_0 = 0x3F8;
		static const uint16_t COM1_PORT_1 = 0x3F9;
		static const uint16_t COM1_PORT_2 = 0x3FA;				/**< FIFO control register base */
		static const uint16_t COM1_PORT_3 = 0x3FB;				/**< */
		static const uint16_t COM1_PORT_4 = 0x3FC;				/**< modem control register base */
		static const uint16_t COM1_PORT_5 = 0x3FD;

		static const uint16_t BAUD_RATE_DIVISOR = 0x02;

		//port1 (interrupt enable register)
		static const uint8_t PORT_1_LOW_POWER_MODE		= 0x20;
		static const uint8_t PORT_1_SLEEP_MODE			= 0x10;
		static const uint8_t PORT_1_MODEM_STATUS		= 0x08;
		static const uint8_t PORT_1_RECEIVER_STATUS		= 0x04;
		static const uint8_t PORT_1_TRANSMITTER_EMPTY	= 0x02;
		static const uint8_t PORT_1_RECEIVED_DATA		= 0x01;
		static const uint8_t PORT_1_DISABLE = 0x00;				/**< everything disabled */

		//port2 (fifo control register)
		static const uint8_t PORT_2_1BYTE		= 0x00;
		static const uint8_t PORT_2_4BYTES		= 0x40;
		static const uint8_t PORT_2_8BYTES		= 0x80;
		static const uint8_t PORT_2_14BYTES		= 0xC0;
		static const uint8_t PORT_2_64BYTE_FIFO	= 0x20;
		static const uint8_t PORT_2_DMA_MODE	= 0x08;
		static const uint8_t PORT_2_CLEAR_TRANS	= 0x04;
		static const uint8_t PORT_2_CLEAR_RECEIV= 0x02;
		static const uint8_t PORT_2_ENABLE_FIFO	= 0x01;
		static const uint8_t PORT_2_MODE =							/**< used configuration */
			PORT_2_ENABLE_FIFO |
			PORT_2_CLEAR_RECEIV |
			PORT_2_CLEAR_TRANS |
			PORT_2_14BYTES;

		//port3 (line control register)
		static const uint8_t PORT_3_DLAB		= 0x80;				/**< divisor access bit (sets port0,port1 to access divisor) */
		static const uint8_t PORT_3_BREAK_ENABLE= 0x40;
		static const uint8_t PORT_3_LOW_PARITY	= 0x38;
		static const uint8_t PORT_3_HIGH_PARITY = 0x28;
		static const uint8_t PORT_3_EVEN_PARITY	= 0x18;
		static const uint8_t PORT_3_ODD_PARITY	= 0x08;
		static const uint8_t PORT_3_NO_PARITY	= 0x00;
		static const uint8_t PORT_3_2_STOP_BITS	= 0x04;
		static const uint8_t PORT_3_1_STOP_BIT	= 0x00;
		static const uint8_t PORT_3_8B_WORD		= 0x03;
		static const uint8_t PORT_3_7B_WORD		= 0x02;
		static const uint8_t PORT_3_6B_WORD		= 0x01;
		static const uint8_t PORT_3_5B_WORD		= 0x00;
		static const uint8_t PORT_3_MODE =							/**< used configuration */
			PORT_3_8B_WORD |
			PORT_3_1_STOP_BIT |
			PORT_3_NO_PARITY;

		//port4 (modem control register)
		static const uint8_t PORT_4_AUTOFLOW_CONTROL = 0x20;	/**< enables autoflow (16750 only) */
		static const uint8_t PORT_4_LOOPBACK_MODE = 0x10;		/**< enables loopback mode */
		static const uint8_t PORT_4_AUX_OUTPUT_2 = 0x08;		/**< enables extern uart cpu (activates interrupts) */
		static const uint8_t PORT_4_AUX_OUTPUT_1 = 0x04;		/**< switch crystal (for MIDI) */
		static const uint8_t PORT_4_REQUEST_TO_SEND = 0x02;		/**< enables sending data (RTS) */
		static const uint8_t PORT_4_DATA_TERMINAL_READY = 0x01;	/**< enables receiving data (DTR) */
		static const uint8_t PORT_4_MODE =					/**< used configuration */
			PORT_4_REQUEST_TO_SEND | PORT_4_DATA_TERMINAL_READY;				

		static const uint8_t SEND_READY = 0x20;

		static const uint8_t RECEIVE_READY = 0x1;

	private:
		/**
		 * Constructor for a serial port.
		 */
		serial_port();

		/**
		 * Destructor
		 */
		virtual ~serial_port();

		/**
		 * Intern writing Method
		 * @param[in] c the char to write
		 */
		void real_write( const char c ) const;
};

}}} //iposix::arch::x86

#endif /* !_KERN_ARCH_X86_DEVICES_SERIAL_PORT_H_ */
