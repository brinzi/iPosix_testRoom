#ifndef _KERN_INCLUDE_DEVICES_I_CONSOLE_H_
#define _KERN_INCLUDE_DEVICES_I_CONSOLE_H_

#include "devices/i_char_device.h"
#include "utils/noncopyable.h"

#include <memory>
#include <string>
#include <stdint.h>

namespace iposix
{
namespace arch
{

class i_console
	: virtual public ::iposix::utils::Noncopyable,
	public ::iposix::arch::i_char_device
{
	public:
		/** Constructor */
		i_console( const ::std::string& name );

		/** Destructor */
		~i_console();

		/**
		 * Reads a char
		 * @param[in] buffer the buffer to write the data to
		 * @param[in] length the length of the buffer
		 * @return the amount of transfered data
		 */
		uint32_t read( void* buffer, uint32_t length );

		/**
		 * Writes chars to normal out
		 * @param[in] buffer to read the data from
		 * @param[in] length the length of the buffer
		 */
		uint32_t write( const void* buffer, uint32_t length );

		/**
		 * Performs a clear on the console
		 */
		void clear();

		static void init();

	private:
};

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_DEVICES_I_CONSOLE_H_ */
