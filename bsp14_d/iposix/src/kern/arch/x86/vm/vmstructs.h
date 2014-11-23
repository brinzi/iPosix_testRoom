#ifndef _KERN_ARCH_X86_VM_VMSTRUCTS_H_
#define _KERN_ARCH_X86_VM_VMSTRUCTS_H_

namespace iposix {
namespace arch {
namespace x86 {
namespace vm {

/**
 * A x86 page table entry
 */
struct __attribute__((packed)) page_table_entry
{
	/**
	 * Struct representing a valid page table entry
	 */
	struct __attribute__((packed)) valid_type
	{
		bool presence:1;			/**< page present (necessary to be valid) */
		bool write:1;				/**< page is writeable */
		bool user_supervisor:1;			/**< page can be read from userlevel (CPL=3) */
		bool write_through:1;			/**< Cache write-through or write-back */
		bool cache_disable:1;			/**< Disable caching */
		bool access:1;				/**< page was accessed (set by hardware) */
		bool dirty:1;				/**< page was written (set by hardware) */
		bool page_table_attribute_index:1;	/**< Page-Table Attribute Index */
		bool global:1;				/**< Global page, no cleared when TLB is flushed */
		unsigned direct_mapped:1; 		/**< this is not mandatory by the hardware.  iPosix special: used for identifying direct mapped pages */
		unsigned avail:2;		/**< Available bits, can be used by the programmer, not used in this implementation */
		unsigned page_frame_address:20;	/**< upper 20 bits of the physical address */
	};

	/**
	 * Struct representing a invalid page table entry
	 */
	struct __attribute__((packed)) invalid_type
	{
		bool presence:1;		/**< page present (necessary to be not set here) */
		unsigned avail:31;		/**< Available bits, can be used by the programmer, not used in this implementation */
	};

	union
	{
		valid_type valid;		/**< used to access the page_table_entry if it's valid */
		invalid_type invalid;		/**< used to access the page_talbe_entry if it's invalid */
		void * flat;			/**< used to access the page_table_entry without interpretation */
	};

	/**
	 * Constructor
	 *
	 * constructs a invalid page_table_entry
	 */
	page_table_entry()
	{
		this->invalid.presence = false;
		this->invalid.avail = 0;
	}

	/**
	 * Returns the physical address behind the page table entry
	 *
	 * @return the physical address
	 */
	void * address()
	{
		return reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( this->flat ) & 0xFFFFF000 );
	}
};

/**
 * A x86 page directory entry
 */
struct __attribute__((packed)) page_dir_entry
{
	/**
	 * Struct representing a valid page table entry
	 */
	struct __attribute__((packed)) valid_type
	{
		bool presence:1;			/**< page present (necessary to be valid) */
		bool write:1;				/**< page is writeable */
		bool user_supervisor:1;			/**< page can be read from userlevel (CPL=3) */
		bool write_through:1;			/**< Cache write-through or write-back */
		bool cache_disable:1;			/**< Disable caching */
		bool access:1;				/**< page was accessed (set by hardware) */
		bool available:1;			/**< Available bits, can be used by the programmer, not used in this implementation */
		bool page_size:1;			/**< page size (0 = 4KiB, 1 = 4MiB) */
		bool global:1;				/**< Global page, no cleared when TLB is flushed */
		unsigned direct_mapped:1; 		/**< this is not mandatory by the hardware.  iPosix special: used for identifying direct mapped pages */
		unsigned avail:2;		/**< Available bits, can be used by the programmer, not used in this implementation */
		unsigned page_frame_address:20;	/**< upper 20 bits of the physical address */
	};

	/**
	 * Struct representing a invalid page table entry
	 */
	struct __attribute__((packed)) invalid_type
	{
		bool presence:1;		/**< page present (necessary to be not set here) */
		unsigned avail:31;		/**< Available bits, can be used by the programmer, not used in this implementation */
	};

	union
	{
		valid_type valid;		/**< used to access the page_table_entry if it's valid */
		invalid_type invalid;		/**< used to access the page_talbe_entry if it's invalid */
		void * flat;			/**< used to access the page_table_entry without interpretation */
	};

	/**
	 * Constructor
	 *
	 * constructs a invalid page_dir_entry
	 */
	page_dir_entry()
	{
		this->invalid.presence = false;
		this->invalid.avail = 0;
	}

	/**
	 * Returns the physical address behind the page directory entry
	 *
	 * @return the physical address
	 */
	void * address()
	{
		return reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( this->flat ) & 0xFFFFF000 );
	}

	/**
	 * Returns the physical address behind the page directory entry, if 4mb page mode is enabled.
	 */
	void * address4m()
	{	
		return reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( this->flat ) & 0xFFE00000 );
	}
};

/**
 * Helper struct to work with x86 virtual addresses
 */
struct __attribute__((packed)) virtual_address
{
	/**
	 * A virtual address divided into it's three components
	 */
	struct div
	{
		unsigned offset:12;		/**< offset of the virtual address (equal in virtual and physical address) */
		unsigned page:10;		/**< index of the page_table_entry in the page table referenced by dir */
		unsigned dir:10;		/**< index of the page table in the page directory table */
	};

	union
	{
		div divided;			/**< splitted virtual address, see div */
		void * normal;			/**< normal virtual address */
	};

	/**
	 * Constructor.
	 *
	 * @param[in] conv virtual address
	 */
	virtual_address( void * conv )
		: normal( conv )
	{ }
};

/**
 * Create a new page table (aligned as needed by the hardware)
 *
 * @return pointer to the new page table
 */
page_table_entry * create_page_table();

/**
 * Create a new page directory table (same as create_page_table() here)
 *
 * @return pointer to the new page directory table
 */
page_dir_entry * create_page_directory_table();

/**
 * Deletes a page table previously created by create_page_table()
 *
 * @param[in] pt pointer to the page table
 */
void delete_page_table( page_table_entry * pt );

/**
 * Deletes a page directory table previously created by create_page_directory_table()
 *
 * @param[in] pdt pointer to the page directory table
 */
void delete_page_directory_table( page_dir_entry * pdt );

}}}} //iposix::arch::x86::vm


#endif /* !_KERN_ARCH_X86_VM_VMSTRUCTS_H_ */
