#ifndef _KERN_INCLUDE_EXCEPTIONS_EXCEPTIONS_H_
#define _KERN_INCLUDE_EXCEPTIONS_EXCEPTIONS_H_

#include <stdint.h>
#include <string>

namespace iposix
{
namespace syscalls
{

/**
 * Describes the various reasons for a page fault. Enumaration from x86 is used.
 * So other architectures musst translate this.
 */
enum page_fault_reason_t
{
	/** read page fault in kernel mode with a not present page */
	kernel_read_page_not_present	= 0,
	/** read page fault in kernel mode with a not accesseble page */
	kernel_read_protection_fault	= 1,
	/** write page fault in kernel mode with a not present page */
	kernel_write_page_not_present	= 2,
	/** write page fault in kernel mode with a not accesseble page */
	kernel_write_protection_fault	= 3,
	/** read page fault in user mode with a not present page */
	user_read_page_not_present		= 4,
	/** read page fault in user mode with a not accesseble page */
	user_read_protection_fault		= 5,
	/** write page fault in user mode with a not present page */
	user_write_page_not_present		= 6,
	/** write page fault in user mode with a not accesseble page */
	user_write_protection_fault		= 7
};

/**
 * Describes a message used to dispatch and hanlde a exception.
 */
struct exception_message
{
	/**
	 * A message which describes the exception
	 */
	const ::std::string& message;

	/**
	 * The exception Number
	 * \todo TODO replace with enum
	 */
	const uint32_t exception_number;

	/**
	 * The error code
	 */
	const union
	{
		/** the numeric error code */
		uint32_t error_code;
		/** translated to page fault errors */
		page_fault_reason_t page_fault_reason;
	};

	/**
	 * The instruction pointer
	 */
	uintptr_t& instruction_address;

	/**
	 * Fault memory address - if any
	 */
	uintptr_t& memory_address;

	/**
	 * Constructor
	 * @param[in] message a message describing the error
	 * @param[in] exception_number the numeric exception number
	 * @param[in] error_code the numeric error code
	 * @param[in] isp the instruction pointer
	 * @param[in] mem_addr the faulting memory address (if any)
	 */
	exception_message(
			const ::std::string& message,
			uint32_t exception_number,
			uint32_t error_code,
			uintptr_t& isp,
			uintptr_t& mem_addr
			)
		: message( message ),
		exception_number( exception_number ),
		error_code( error_code ),
		instruction_address( isp ),
		memory_address( mem_addr )
	{ }
};

} //namespace syscalls
} //namespace iposix

#endif /* !_KERN_INCLUDE_EXCEPTIONS_EXCEPTIONS_H_ */
