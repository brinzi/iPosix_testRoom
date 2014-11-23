#ifndef _KERN_INCLUDE_SYSCALLS_SYSCALL_DISPATCHER_H_
#define _KERN_INCLUDE_SYSCALLS_SYSCALL_DISPATCHER_H_

#include "utils/singleton.hpp"

#include <stdint.h>

namespace iposix
{
namespace syscalls
{

class syscall_message;
class exception_message;

class syscall_dispatcher
	: virtual public ::iposix::utils::Singleton< syscall_dispatcher >
{
	friend class ::iposix::utils::Singleton< syscall_dispatcher >;

	public:
		/**
		 * Dispatches an syscall message to the handling object
		 * @param[in,out] msg the coresponding syscall message
		 */
		void dispatch_syscall( syscall_message& msg );

		/**
		 * Dispatches an exception message to the handling objects
		 */
		void dispatch_exception( exception_message& msg );

	private:
		syscall_dispatcher();
		~syscall_dispatcher();
};

} //namespace syscalls
} //namespace iposix

#endif /* !_KERN_INCLUDE_SYSCALLS_SYSCALL_DISPATCHER_H_ */
