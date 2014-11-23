#ifndef _KERN_ARCH_X86_SCHEDULER_PROCESS_H_
#define _KERN_ARCH_X86_SCHEDULER_PROCESS_H_

#include "utils/noncopyable.h"
#include <stdint.h>
#include <vector>
#include "x86/vm/vmstructs.h"
#include "x86/scheduler/process_schedule_data.h"
#include "logger/logger.h"
#include "die.h"

//NOTICE: virtual_address can be implicitly constructed from void *, so this implementation satisfies the policy requirement

using std::vector;

namespace iposix {

namespace scheduler {
	template< typename T > class i_process;
}

namespace arch {
namespace x86 {

using vm::virtual_address;
using vm::page_table_entry;
using vm::page_dir_entry;

/**
 * Hardware related policy for i_process.
 *
 * See policy requirements for hardware process.
 */
struct process
	: virtual public ::iposix::utils::Noncopyable,
	public process_schedule_data
{
	friend class ::iposix::scheduler::i_process< process >;	/**< only ::iposix::scheduler::i_process can use this */

	static page_dir_entry * direct_mapping;			/**< 1:1 mapping for virtual memory */

	enum
	{
		page_size = 4096	/**< page size of the virtual memory system of the hardware */
	};

	/**
	 * Add memory to virtual address table
	 *
	 * @param[in] addresses vector of addresses to be mapped (in order) in the virtual address table
	 * @param[in] vaddr virtual address where to map the memory
	 *
	 * @return virtual address where memory was mapped, or 0 if an error occured
	 */
	void * add_virtual_memory( const vector< void * >& addresses, virtual_address vaddr = 0 );
	/**
	 * Get corresponding physical address to a virtual address
	 *
	 * @param[in] vaddr virtual address to be looked up
	 * @param[in,out] entry_out pointer to a page_table_entry which will be set to the page table entry for vaddr, if not 0
	 *
	 * @return physical address
	 */
	void * get_phys_address( virtual_address vaddr, page_table_entry * entry_out = 0 ) const;
	/**
	 * Removes memory form virtual address table
	 *
	 * @param[in] vaddr virtual address of the memory to be removed
	 *
	 * @return true, if successfull
	 */
	bool remove_virtual_memory( virtual_address vaddr );
	/**
	 * Set a specific page unpresent (it was swapped out). Does currently nothing in iposix.
	 *
	 * @param[in] vaddr Address of the page to be set unpresent
	 */
	void unpresent( virtual_address vaddr );
	/**
	 * Set a specific page present (swapped in). Does currently nothing in iposix.
	 *
	 * @param[in] vaddr Address of the page to be set present.
	 * @param[in] addr physical address of the page
	 */
	void present( virtual_address vaddr, void * addr );
	/**
	 * Copy a memory area from a physical address to a virtual address of this process
	 *
	 * @param[in] vaddr virtual address of the destination
	 * @param[in] addr physical address of the source
	 * @param[in] size size of the memory block
	 * @param[in] copyout do a copy out instead of a copy_in, ONLY for internal uses
	 *
	 * @return true, if successfull.
	 */
	bool copy_in( virtual_address vaddr, void * addr, size_t size, bool copyout = false );
	/**
	 * Copy a memory area from the virtual address space to a physical address
	 *
	 * @param[in] addr physical address of the destination
	 * @param[in] vaddr virtual address of the source
	 * @param[in] size size of the memory block
	 *
	 * @return true, if successfull
	 */
	bool copy_out( void * addr, virtual_address vaddr, size_t size )
	{
		return copy_in( vaddr, addr, size, true );
	}
	/**
	 * Copy the whole address space of a process
	 *
	 * @param[in] proc process whose address space should be copied
	 *
	 * @return true, if successfull
	 */
	bool copy_address_space( const process& proc );

	/**
	 * Does all stuff neccessary for running
	 *
	 * @return true on success
	 */
	bool prepare_running();

	/**
	 * Returns a pointer to the page table
	 *
	 * @return the pointer
	 */
	uintptr_t page_table_ptr();

	bool reset_address_space();

	/**
	 * Create a logger output with verbose information of the virtual address space
	 */
	void dump_vm();

   private:
	page_dir_entry * ptable;		/**< page directory table */
	page_dir_entry * ptable_end;		/**< end of the page directory table (for iterations) */

	/**
	 * Constructor.
	 *
	 * @param[in] kernel is this a kernel process?
	 */
	process( bool kernel = false );
	/**
	 * Destructor.
	 */
	~process();
};

}}} //iposix::arch:x86

#endif /* !_KERN_ARCH_X86_SCHEDULER_PROCESS_H_ */
