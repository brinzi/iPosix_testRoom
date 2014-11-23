#ifndef _KERN_INCLUDE_ELF_I_ELF_LOADER_H_
#define _KERN_INCLUDE_ELF_I_ELF_LOADER_H_

#include "config/elf_loader.h"
#include "scheduler/i_process.h"
#include "utils/singleton.hpp"

#include <string>
#include <memory>
#include <stdint.h>

namespace iposix
{

/* Predefine */
namespace fs
{
class i_fs_node;
} //namespace fs

/**
 * Contains the ELF-File loader
 */
namespace elf
{

using ::iposix::scheduler::process_type;

/**
 * Describes a class which can load an ELF-File
 * More Information at:
 * http://wiki.osdev.org/ELF#Loading_ELF_Binaries
 * http://www.linux-kernel.de/appendix/ap05.pdf
 */
template< typename Arch_Elf_Loader >
class i_elf_loader
	: public Arch_Elf_Loader,
	virtual public ::iposix::utils::Singleton< i_elf_loader< Arch_Elf_Loader > >
{
	friend class ::iposix::utils::Singleton< i_elf_loader< Arch_Elf_Loader > >;

	typedef ::std::shared_ptr< ::iposix::fs::i_fs_node > i_fs_node_ptr;

	public:
		/**
		 * Loads an ELF-File to memory.
		 * @param[in] file_name the name of the file to execute
		 * @param[in] args the arguments passed to the process (null terminated
		 * list of null terminated char arrays)
		 * @return true on success else false
		 */
		bool load_elf(
				const ::std::string& file_name,
				char* const args[]
				);

	private:
		/** Constructor */
		i_elf_loader();

		/** Destructor */
		~i_elf_loader();

		/**
		 * Dummy function (used til vm is ready - or should call vm). Allocates
		 * a memory region in virtual memory.
		 * @param[in] process the process for which the area should be allocated
		 * @param[in] vaddr the virtual address to allocate
		 * @param[in] size the size of the memory region
		 * @param[in] align the alignment
		 * @param[in] read is the region readable
		 * @param[in] write is the region writeable
		 * @param[in] execute is the region executable
		 * @return true on succes else false
		 */
		bool allocate_area(
				process_type& process,
				uintptr_t vaddr,
				uint64_t size,
				uint64_t align,
				bool read,
				bool write,
				bool execute
				);

		/**
		 * Allocates a stack for the process
		 * @param[in] process the process to allocate the stack for
		 * @return true on success
		 */
		bool allocate_stack(
				process_type& process
				);

		/**
		 * Dummy function (used til vm is ready - or should call vm). Copies the
		 * content of a file to the virtual memory.
		 * @param[in] process the process in which address space the content
		 * should be copied to
		 * @param[in] file the file to read from
		 * @param[in] file_offset the offset to the files beginning
		 * @param[in] vaddr the virtual address to write to
		 * @param[in] size the length of the content
		 * @return true on succes else false
		 */
		bool copy_to_area(
				process_type& process,
				i_fs_node_ptr file,
				uint64_t file_offset,
				uintptr_t vaddr,
				uint64_t size
				);

		static const uintptr_t INITIAL_STACK_BASE = 0 - Arch_Elf_Loader::INITIAL_ELF_STACKSIZE;
		static const uintptr_t INITIAL_STACK_POINTER = 0 - sizeof( intptr_t );
};

typedef i_elf_loader< ARCH_ELF_LOADER > elf_loader_type;

} //namespace elf
} //namespace iposix

#endif /* !_KERN_INCLUDE_ELF_I_ELF_LOADER_H_ */
