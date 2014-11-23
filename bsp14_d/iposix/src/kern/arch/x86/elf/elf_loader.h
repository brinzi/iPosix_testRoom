#ifndef _KERN_ARCH_X86_ELF_ELF_LOADER_H_
#define _KERN_ARCH_X86_ELF_ELF_LOADER_H_

#include "scheduler/i_process.h"
#include "utils/singleton.hpp"

#include <stdint.h>

namespace iposix
{

/* Predefine */
namespace elf
{
template< typename Arch_Elf_Loader >
class i_elf_loader;
}//namespace elf

namespace arch
{
namespace x86
{

using ::iposix::scheduler::process_type;

class elf_loader
	: virtual public ::iposix::utils::Singleton< elf_loader >
{
	friend class ::iposix::elf::i_elf_loader< elf_loader >;

	public:
		/** Does the ELF Loader supports 32bit executables */
		static const bool ELF32_SUPPORT = true;

		/** Does the ELF Loader supports 64bit executables */
		static const bool ELF64_SUPPORT = false;

		static const uint32_t STACK_ALIGNMENT = 4096;
		static const uint32_t INITIAL_ELF_STACKSIZE = 4096;

		/**
		 * Performs a jump with changing the stack pointer, the instruction
		 * pointer, the segment selectors and copies the arguments to the new
		 * stack.
		 * @param[in] process the process to prepare for running
		 * @param[in] stack_pointer the new stack pointer
		 * @param[in] entry_point the new instruction pointer
		 * @param[in] args the argument to push on the stack
		 * should be used and false if the user space once should be used
		 */
		bool jump_in(
				process_type& process,
				uintptr_t stack_pointer,
				uint32_t entry_point, 
				char* const args[]
				);

	private:
		elf_loader();

		~elf_loader();
};

} //namespace x86
} //namespace arch
} //namespace iposix


#endif /* !_KERN_ARCH_X86_ELF_ELF_LOADER_H_ */
