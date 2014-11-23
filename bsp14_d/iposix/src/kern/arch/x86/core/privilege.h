#ifndef _KERN_ARCH_X86_CORE_PRIVILEGE_H_
#define _KERN_ARCH_X86_CORE_PRIVILEGE_H_

#include <stdint.h>

namespace iposix
{
namespace arch
{
namespace x86
{

struct privilege
{
	/**
	 * Defines the Privilege Level. It descripes who can use something or
	 * which is the systems current privilege level.
	 */
	enum level
	{
		/**
		 * An Interrupt that cannot be accessed by User Mode processes, it can only
		 * be accesssed by System Mode processes (ring 0). 
		 */
		system_privilege_level = 0,

		/**
		 * Unused in common systems. This kind of Interrupts can be accessed by
		 * processes running in ring 1 or higher.
		 */
		unused_level_1 = 1,

		/**
		 * Unused in common systems. This kind of Interrupts can be accessed by
		 * proccess runnning in ring 2 or higher.
		 */
		unused_level_2 = 2,

		/**
		 * An Interrupt that can be accessed by User Mode processes. (ring 3 or
		 * higher)
		 */
		user_privilege_level = 3
	};
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_CORE_PRIVILEGE_H_ */
