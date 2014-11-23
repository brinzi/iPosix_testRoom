#ifndef _COMMON_INCLUDE_SYSCALLS_SYSCALLER_H_
#define _COMMON_INCLUDE_SYSCALLS_SYSCALLER_H_

#include "syscalls.h"

namespace iposix
{
namespace syscalls
{

class syscaller
{
	public:
		static void call_system(syscall_message& msg);
};

} //namespace syscall
} //namespace iposix

#endif /* !_COMMON_INCLUDE_SYSCALLS_SYSCALLER_H_ */
