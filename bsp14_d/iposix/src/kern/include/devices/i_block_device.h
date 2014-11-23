#ifndef _KERN_INCLUDE_DEVICES_H_I_BLOCK_DEVICE_H_
#define _KERN_INCLUDE_DEVICES_H_I_BLOCK_DEVICE_H_

#include "utils/noncopyable.h"

#include <stdint.h>
#include <memory>
#include <string>

namespace iposix {
namespace arch {

/**
 * Describes an abstract block oriented device.
 */
class i_block_device
	: virtual public ::iposix::utils::Noncopyable
{
	public:
		i_block_device( const ::std::string& name );

		virtual ~i_block_device() { };

		/**
		 * Reads data from this drive.
		 * @param[in] base_address the base_address on the harddisk to read from
		 * @param[in,out] buffer the buffer where to write the data to
		 * @param[in] length the length of the buffer (in bytes)
		 * @return the number of transfered bytes (-1 on errors)
		 */
		int32_t read(
				uint64_t base_address, 
				void* buffer, 
				uint32_t length );

		/**
		 * Writes data to this drive.
		 * @param[in] base_address the base_address on the harddisk to write to
		 * @param[in,out] buffer the data to read the data from
		 * @param[in] length the length of the buffer (in bytes)
		 * @return the number of transfered bytes (-1 on errors)
		 */
		int32_t write(
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
			return false;
		}

		/**
		 * Returns the size of a sector
		 * @return the size
		 */
		virtual uint32_t block_size() const = 0;

		/**
		 * Returns the length in Bytes
		 * @return the length
		 */
		virtual uint64_t length() const = 0;

		/**
		 * Reads data from this drive.
		 * @param[in] base_sector the base_sector on the harddisk to read from
		 * @param[in,out] buffer the buffer where to write the data to
		 * @param[in] length the length of the buffer (in blocks)
		 * @return the number of transfered blocks (-1 on errors)
		 */
		virtual int32_t read_block(
				uint64_t base_sector, 
				void* buffer, 
				uint32_t length ) = 0;

		/**
		 * Writes data to this drive.
		 * @param[in] base_sector the base_sector on the harddisk to write to
		 * @param[in,out] buffer the data to read the data from
		 * @param[in] length the length of the buffer (in blocks)
		 * @return the number of transfered blocks (-1 on errors)
		 */
		virtual int32_t write_block(
				uint64_t base_sector, 
				const void* buffer, 
				uint32_t length ) = 0;
};

typedef ::std::shared_ptr< i_block_device > i_block_device_ptr;

} //namespace arch
} //namespace iposix

#endif /* !_KERN_INCLUDE_DEVICES_H_I_BLOCK_DEVICE_H_ */
