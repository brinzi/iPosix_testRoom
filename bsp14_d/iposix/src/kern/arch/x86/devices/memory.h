#ifndef _KERN_ARCH_X86_DEVICES_MEMORY_H_
#define _KERN_ARCH_X86_DEVICES_MEMORY_H_

#include <stdint.h>
#include "elf/elf.h"
#include "mm/memory_area.h"
#include "utils/singleton.hpp"

namespace iposix
{

using hlmemory::memory_area;

namespace arch
{
namespace x86
{

/**
 * A x86 implementation of a memory policy for iposix_kernel
 */
class memory
	: ::iposix::utils::Singleton< memory >
{
	friend class ::iposix::utils::Singleton< memory >;

	public:
		/**
		 * Initializes the Memory Descriptor Tables and stuff like that
		 */
		void init();

		/**
		 * Get the memory areas for the memory managements
		 *
		 * @param[in,out] buffer a pointer to an empty memory_area array
		 * @param[in] size size of the buffer
		 *
		 * @return count of memory areas written to the buffer
		 */
		uint32_t get_memory_areas( memory_area * buffer, uint32_t size ) const;

		/**
		 * Invalides all caches
		 */
		void invalidate_cache() const;

		/**
		 * Invalidates the full CPUs TLB cache
		 */
		void invalidate_translation_lookaside_buffer() const;

		/**
		 * Invalidates a specific address in the CPUs cache
		 * \todo implement
		 * @param[in] addr the address
		 */
		void invalidate_translation_lookaside_buffer( const void* addr ) const;

		/**
		 * Sets up the Hardware register to use the pagetable
		 * @param[in] pagetable_ptr a pointer to the pagetable
		 */
		void set_page_table( void* pagetable_ptr ) const;

		/**
		 * Enables or Disable Paging
		 * @param[in] enable if true paging is enabled else disabled
		 */
		void paging( bool enable ) const;

		/**
		 * Returns wheter paging is enabled or not
		 *
		 * @return true, if paging is enabled
		 */
		bool paging() const;

		/**
		 * Enables the page size and global pages.
		 */
		void init_cr4() const;

	private:
		/**
		 * Creates an instance of the memory interface.
		 */
		memory();

		/**
		 * Destructor
		 */
		~memory();

		memory_area areas[10];	/**< buffer for memory areas */
		uint8_t area_count;	/**< count of memory areas in the buffer */
};

/**
 * Struct that switches pagin off in current scope like a lock.
 * On creation paging is switched off and on deletion paging is
 * switched on.
 */
struct scoped_paging_off
{
	/**
	 * Constructor. Switches paging off.
	 * @param[in] preserve_old_value should the old value be saved
	 */
	scoped_paging_off( bool preserve_old_value );

	/**
	 * Destructor. Switches paging on or restores the old value.
	 */
	~scoped_paging_off();

	private:
	/** the old value */
	bool old_value;
};

} //namespace x86
} //namespace arch
} //namespace iposix

#endif /* !_KERN_ARCH_X86_DEVICES_MEMORY_H_ */
