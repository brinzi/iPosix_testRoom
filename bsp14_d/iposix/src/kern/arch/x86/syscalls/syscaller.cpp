#include "syscalls/syscaller.h"

namespace iposix
{
namespace syscalls
{

void syscaller::call_system(syscall_message& msg)
{
	//do syscall
	asm volatile("int $128"
		:
		: "a"(&msg)
		: "memory" );
}

} //namespace syscall
} //namespace iposix
