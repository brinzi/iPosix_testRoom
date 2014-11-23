#ifndef _KERN_INCLUDE_DEVICES_I_CHAR_DEVICE_H_
#define _KERN_INCLUDE_DEVICES_I_CHAR_DEVICE_H_

#include "utils/noncopyable.h"

#include <stdint.h>
#include <memory>
#include <string>

namespace iposix {
namespace arch {

/**
 * Describes an abstract character device.
 */
class i_char_device
	: virtual public ::iposix::utils::Noncopyable
{
	public:
		i_char_device( const ::std::string& name );

		virtual ~i_char_device() { };

		/**
		 * Reads data from this device.
		 * @param[in] base_address musst be zero
		 * @param[in,out] buffer the buffer where to write the data to
		 * @param[in] length the length of the buffer (in bytes)
		 */
		uint32_t read(
				uint64_t base_address, 
				void* buffer, 
				uint32_t length );

		/**
		 * Writes data to this device.
		 * @param[in] base_address musst be zero
		 * @param[in,out] buffer the data to read the data from
		 * @param[in] length the length of the buffer
		 */
		uint32_t write(
				uint64_t base_address, 
				const void* buffer, 
				uint32_t length );

		/**
		 * The name of this drive
		 */
		const ::std::string name;

		/**
		 * Returns if this device is a char device
		 * @return true
		 */
		inline bool is_tty() const
		{
			return true;
		}

		/**
		 * Reads data from this device.
		 * @param[in,out] buffer the buffer where to write the data to
		 * @param[in] length the length of the buffer (in bytes)
		 */
		virtual uint32_t read(
				void* buffer, 
				uint32_t length ) = 0;

		/**
		 * Writes data to this device.
		 * @param[in,out] buffer the data to read the data from
		 * @param[in] length the length of the buffer
		 */
		virtual uint32_t write(
				const void* buffer, 
				uint32_t length ) = 0;

		/**
		 * Performs a clear on the device - if possible
		 */
		virtual void clear() = 0;
};

typedef ::std::shared_ptr< i_char_device > i_char_device_ptr;

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_DEVICES_I_CHAR_DEVICE_H_ */
