#ifndef _KERN_INCLUDE_MM_POLICIES_SPLITTED_HPP_
#define _KERN_INCLUDE_MM_POLICIES_SPLITTED_HPP_

#include "mm/memory_area.h"
#include "utils/singleton.hpp"

#include "logger/logger.h"

#include <vector>
#include <stdint.h>

#include "scheduler/i_process.h"

namespace iposix {
namespace hlmemory {
namespace policies {

using scheduler::process_type;

//#define VERBOSE_ALLOC_FREE


/**
 * Idea:
 * HEAD                                                 TAIL
 * |-------------------------------------------------------|
 * ^      ^        ^                             ^         ^
 * |      |        |                             |         |
 * |      |  max_kernel_bound                    |         |
 * |      |                                      |         |
 * |      cur_kernel_bound          cur_user_bound         |
 * |                                                       |
 * first_address                                last_address
 *
 * Like u see there are two constant bounds first_address and last_address these
 * bounds describe the useable memory (assumping there is one sequential memory
 * region with no holes).
 * There are two different strategies used for manage the memory and therefore
 * we have two regions to manage depending on who want to allocate memory:
 * 1. The kernel:
 *    Kernel memory is managed with free/used structures. These structures hold
 *    the following information:
 *    - does these structure describe a free or an used area
 *    - a pointer to the previous structure
 *    - a pointer to the next structure
 *    - a pointer to the underlying allocated memory
 *    - the size of the underlying allocated memory
 * 2. The userspace:
 *    Userspace memory is managed in page size. There are two possibilities free or
 *    allocate a full page, so we need to know which pages are free or not,
 *    therefore we use a bitmap.
 * Additional we remember the current and maximal bounds for the kernel and
 * userspace memory. This is easy since we allocate kernel memory from head
 * an userspace memory from the tail.
 */
template< uint32_t PageSize = 0 >
class splitted
	: virtual public ::iposix::utils::Singleton< splitted< PageSize > >
{
	friend class ::iposix::utils::Singleton< splitted< PageSize > >;

	/**
	 * A Bitmap which holds booleans this is not performance optimized this is
	 * space optimized
	 */
	class bitmap
	{
		public:
			/**
			 * Constructs a empty bitmap
			 */
			bitmap()
				: data( 0 ),
				mem_size( 0 ),
				entrys( 0 )
		{ }

			/**
			 * A pseudo value which gives bit-access to outside and pretends to
			 * ba a boolean
			 */
			struct bitmap_bool
			{
				/**
				 * Only the bitmap class can construct such a value.
				 */
				friend class bitmap;

				public:
				/** Destructor */
				~bitmap_bool()
				{ }

				/**
				 * Return if the page is free
				 * @return ture if the page is free
				 */
				bool free()
				{
					//0 => free
					//1 => not free
					return ( ( (this->data) & (this->free_mask) ) != (this->free_mask) );
				}

				/**
				 * Sets if the page is free
				 * @param[in] is_free true if the page is free
				 */
				void free( bool is_free )
				{
					//0 => free
					//1 => not free
					if ( is_free )
					{
						//unset bit
						this->data &= ~(this->free_mask);
					}
					else
					{
						//set bit
						this->data |= (this->free_mask);
					}
				}

				/**
				 * Return if the page is a kernel page
				 * @return ture if the page is a kernel page
				 */
				bool kernel()
				{
					//0 => not kernel
					//1 => kernel
					return ( ( (this->data) & (this->kernel_mask) ) == (this->kernel_mask) );
				}

				/**
				 * Sets if the page is a kernel page
				 * @param[in] is_kernel true if the page is a kernel page
				 */
				void kernel( bool is_kernel )
				{
					//0 => not kernel
					//1 => kernel
					if ( is_kernel )
					{
						//set bit
						this->data |= (this->kernel_mask);
					}
					else
					{
						//unset bit
						this->data &= ~(this->kernel_mask);
					}
				}

				private:
				/**
				 * Constructor
				 * @param[in] data refernece to the real value
				 * @param[in] free_mask a bitmask which is used to access the free-bit
				 * @param[in] kernel_mask a bitmask which is used to access the kernel-bit
				 */
				bitmap_bool(
						uint8_t& data,
						uint8_t free_mask,
						uint8_t kernel_mask )
					: data( data ),
					free_mask( free_mask ),
					kernel_mask( kernel_mask )
				{ }


				public:
				/** Reference to the value */
				uint8_t& data;
				/** A bitmask for free-bit access */
				uint8_t free_mask;
				/** A bitmaks for kernel-bit access */
				uint8_t kernel_mask;
			};

			/**
			 * Sets up the bitmap to use a specifice memory region for
			 * datastructures
			 * @param[in] addr pointer to the memory region where the data is
			 * located at
			 * @param[in] entrys the number of entrys to manage
			 * @return the amount of memory which is used by the bitmap
			 */
			uint64_t set_to( void* addr, uint64_t entrys )
			{
				this->data = reinterpret_cast<uint8_t*>( addr );
				// we need two bits for every page so device by 4
				this->mem_size = entrys >> 2; // div 4
				//cause we put the map at the pagining of the memory we could
				//not use this space as pages
				this->entrys = entrys - ( ( this->mem_size / PageSize ) + 1 );

				//set everything to not kernel and not used
				for ( uint64_t i = 0; i < entrys; i++ )
				{
					data[i] = 0;
				}
				
				return this->mem_size;
			}

			/**
			 * Array operator to access the bits
			 * @param[in] entry the entry index
			 * @return a boolean like value
			 */
			bitmap_bool operator[]( ::std::size_t entry )
			{
				/*	entry	index	free_mask	kernel_mask		n		n2
					0		0		00000001	00000010		0		0
					1		0		00000100	00001000		2		2
					2		0		00010000	00100000		4		4
					3		0		01000000	10000000		6		6
					4		1		00000001	00000010		8		0
					5		1		00000100	00001000		10		2
					6		1		00010000	00100000		12		4
					7		1		01000000	10000000		14		6
					8		2		00000001	00000010		16		0
					=>
					e		e/4		1 << (e*2)	2 << (e*2)		e*2		(e*2)%8
					=>
					e		e>>2	1 << (n2)	2 << (n2)		e<<1	(e<<1)&7
				*/
				return bitmap_bool(
						data[ entry >> 2 ],				/* entry / 4 */
						1 << ( ( entry << 1 ) & 0x7 ),	/* 1 << ( entry * 2 ) % 8 */
						2 << ( ( entry << 1 ) & 0x7 )	/* 2 << ( entry * 2 ) % 8 */
						);
			}

			/**
			 * Returns the amount of memroy used by the bitmap
			 * @return the amount
			 */
			uint64_t size()
			{
				return this->mem_size;
			}

			/**
			 * Returns the length (number of entrys) in the bitmap
			 * @return the number
			 */
			uint64_t length()
			{
				return this->entrys;
			}

		private:
			/** A pointer to the data */
			uint8_t* data;
			/** The amount of memory used by the bitmap */
			uint64_t mem_size;
			/** The number of entrys */
			uint64_t entrys;
	};

	/**
	 * Type to ease operations on addresses
	 */
	union address_t
	{
		void* v_ptr;
		uintptr_t int_ptr;
		address_t ( void* v  = 0)
			: v_ptr( v )
		{ }
	};

	/**
	 * Type to manage kernel memory free/used_structure. This is a dual-directed
	 * list.
	 */
	struct fu_structure
	{
		/** Is this a free region */
		bool free;
		/** The previous structure in order */
		fu_structure* previous;
		/** The next structure in order */
		fu_structure* next;
		/** Pointer to the described memory region */
		address_t allocated_memory;
		/** Size of the described memory region */
		uint64_t size;
	};

	public:

		/**
		 * Allocates memory
		 * @param[in] size the amount of memory to allocate
		 * @param[in] kernel is this kernel memory
		 * @param[in] aligned should the region be aligned (Note: user memory is
		 * always aligned)
		 * @return a pointer to the allocated region (if any or zero else)
		 */
		void* malloc( uint64_t size, bool kernel, bool aligned );

		/**
		 * Free memory
		 * @param[in] addr the memory address to free
		 * @param[in] kernel is this kernel memory
		 */
		bool free( void* addr, bool kernel );

		/**
		 * sets the a memory to manage
		 */
		template< typename T >
		void set_memory_areas( const T& mem );

		void * vmalloc( uint32_t, process_type::hw_type&, void * );

		/**
		 * Returns if the memory management is initialized
		 * @return true if initialized
		 */
		bool initialized()
		{
			return this->is_initialized;
		}

	private:
		/** Constructor */
		splitted()
			: is_initialized( false ),
			first_address( 0 ),
			cur_kernel_bound( 0 ),
			max_kernel_bound( 0 ),
			cur_user_bound( 0 ),
			last_address( 0 ),
			page_bm(),
			first_structure( 0 )
		{ }

		/** Allready initialized */
		bool is_initialized;

		/** first usable address */
		address_t first_address;
		/** current kernel bound */
		address_t cur_kernel_bound;
		/** maximal kernel bound */
		address_t max_kernel_bound;
		/** current user bound */
		address_t cur_user_bound;
		/** last usable address */
		address_t last_address;

		/** Bitmap used for managing page allocations */
		bitmap page_bm;

		/** List used for managing small allocations */
		fu_structure* first_structure;

		/** real allocation function used by malloc for kernel allocation */
		inline void* kernel_malloc( uint64_t size );

		/** real freeing function used by free for kernel freeing */
		inline bool kernel_free( void* addr );

		/** real allocation function used by malloc for user allocation */
		inline void* page_alloc( bool kernel );

		/** real freeing function used by free for user freeing */
		inline bool page_free( void* addr );
};

template< uint32_t PageSize >
template< typename T >
void splitted< PageSize >::set_memory_areas( const T& mem )
{
	if ( !this->is_initialized )
	{
		memory_area area;
		mem.get_memory_areas( &area, 1 );

		address_t low( area.low );
		address_t high( area.high );
		address_t bm_addr( area.low );
		uint64_t bm_entrys = ( high.int_ptr - low.int_ptr ) / PageSize ;

		uint64_t bm_needed_size = this->page_bm.set_to( bm_addr.v_ptr, bm_entrys );

		low.int_ptr += bm_needed_size;

		if ( low.int_ptr % PageSize != 0 )
		{
			auto leak = PageSize - ( low.int_ptr % PageSize );
			low.int_ptr += leak;
		}

		if ( high.int_ptr % PageSize != 0 )
		{
			auto leak = high.int_ptr % PageSize;
			high.int_ptr -= leak;
		}

		this->first_address.int_ptr = low.int_ptr;
		this->cur_kernel_bound = this->first_address;
		this->max_kernel_bound = this->first_address;

		this->cur_user_bound = area.high;
		this->last_address = area.high;

		this->is_initialized = true;

		logger << message::MORE_VERBOSE << __func__ << ":" << endl
			<< "\tfirst_memory address:" << hex << low.int_ptr << endl
			<< "\tbitmap mem_size:" << dec << this->page_bm.size() << endl
			<< "\tbitmap length  :" << dec << this->page_bm.length() << endl
			<< "\tfirst usable address:" << hex << this->first_address.int_ptr << endl
			<< "\tlast usable address:" << hex << this->last_address.int_ptr << endl;
	}
}

template< uint32_t PageSize >
void* splitted< PageSize >::malloc(
		uint64_t size,
		bool kernel,
		bool aligned = false
		)
{
	if ( this->is_initialized )
	{
		void* ret( 0 );

#ifdef VERBOSE_ALLOC_FREE
		logger << message::MORE_VERBOSE << __func__
			<< ": size=" << dec << size
			<< ( aligned ? " aligned" : " not aligned" )
			<< ( kernel ? " as kernel" : " not as kernel" )
			<< endl;
#endif //VERBOSE_ALLOC_FREE

		if ( kernel && !aligned )
		{
			ret = kernel_malloc( size );
		}
		else
		{
			//less then a page is okay
			size = size > PageSize ? size : PageSize;

			if ( size == PageSize )
			{
				ret = page_alloc( kernel );
			}
			else
			{
				logger << message::MORE_VERBOSE << __func__
					<< ": Could not return aligned memory with size " << size
					<< " which is not of PageSize size " << PageSize
					<< endl;
			}
		}

#ifdef VERBOSE_ALLOC_FREE
		logger << message::MORE_VERBOSE << __func__
			<< ": ret=" << hex << reinterpret_cast<intptr_t>(ret) << endl;
#endif //VERBOSE_ALLOC_FREE

		return ret;
	}
	else
	{
		return 0;
	}
}

template< uint32_t PageSize >
bool splitted< PageSize >::free( void* addr, bool kernel )
{
	if ( this->is_initialized )
	{
		bool ret( false );
		address_t free_addr( addr );

		if (
				kernel &&
				( free_addr.int_ptr >= first_address.int_ptr ) &&
				( free_addr.int_ptr <= cur_kernel_bound.int_ptr )
		   )
		{
			ret = kernel_free( addr );
		}
		else if (
				!kernel &&
				( free_addr.int_ptr >= cur_user_bound.int_ptr ) &&
				( free_addr.int_ptr <= last_address.int_ptr )
				)
		{
			ret = page_free( addr );
		}

		return ret;
	}
	else
	{
		return false;
	}
}

template< uint32_t PageSize >
void* splitted< PageSize >::kernel_malloc( uint64_t size )
{
	splitted::fu_structure* cur_structure = this->first_structure;
	splitted::fu_structure* prev_structure = 0;

	//iterate through the structure chain
	while (
			( cur_structure ) && //not null
			!(
				( cur_structure->free ) && //free
				( cur_structure->size >= size ) //enough
			 )
			)
	{
		prev_structure = cur_structure;
		cur_structure = cur_structure->next;
	}

	address_t ret( 0 );
	if ( !cur_structure )
	{
#ifdef VERBOSE_ALLOC_FREE
		logger << message::MORE_VERBOSE << __func__
			<< ": creating new fu_structure " << endl;
#endif //VERBOSE_ALLOC_FREE

		//end reached
		address_t new_kernel_bound = this->cur_kernel_bound;
		new_kernel_bound.int_ptr += sizeof( fu_structure );
		new_kernel_bound.int_ptr += size;

		if ( new_kernel_bound.int_ptr > cur_user_bound.int_ptr )
		{
			//there is no memory available
			return 0;
		}
		else if ( new_kernel_bound.int_ptr > max_kernel_bound.int_ptr )
		{
			max_kernel_bound = new_kernel_bound;
		}

		//create a new structure
		cur_structure = reinterpret_cast<fu_structure*>( this->cur_kernel_bound.v_ptr );
		cur_structure->free = false;
		cur_structure->previous = prev_structure; //may be 0
		cur_structure->next = 0;
		cur_structure->allocated_memory = cur_kernel_bound;
		cur_structure->allocated_memory.int_ptr += sizeof( fu_structure );
		cur_structure->size = size;

		cur_kernel_bound = new_kernel_bound;
		if ( !this->first_structure )
		{
			//there is no first structure we created the first
			this->first_structure = cur_structure;
		}
		else if ( prev_structure )
		{
			prev_structure->next = cur_structure;
		}

		ret = cur_structure->allocated_memory;
	}
	else if ( cur_structure->free && ( cur_structure->size >= size ) )
	{
		//free and enough
		cur_structure->free = false;

		if ( cur_structure->size > size )
		{
			// more then we need
			uint64_t remain = cur_structure->size - size;
			if  ( remain > sizeof( fu_structure ) )
			{
				// the rest big enough to hold a new fu_structure
				remain -= sizeof( fu_structure );
				cur_structure->size = size;

				address_t new_fu_struct_pos = cur_structure->allocated_memory;
				new_fu_struct_pos.int_ptr += size;

				// create one
				fu_structure* new_fu = reinterpret_cast<fu_structure*>(new_fu_struct_pos.v_ptr);
				new_fu->free = true;
				new_fu->previous = cur_structure; // new_fus previous is the current
				new_fu->next = cur_structure->next; //new_fus next is the currents next
				new_fu->allocated_memory = new_fu_struct_pos;
				new_fu->allocated_memory.int_ptr += sizeof( fu_structure );
				new_fu->size = remain;

				//lets put new_fu in beetween current and next
				//currents nexts previous is the new_fu
				cur_structure->next->previous = new_fu;
				//currents next is the new_fu
				cur_structure->next = new_fu;
			}
		}
		ret = cur_structure->allocated_memory;
	}
	else
	{
		//internal error
		logger << message::ERROR << __func__
			<< ": Internal Error" << endl;
		return 0;
	}

	return ret.v_ptr;
}

template< uint32_t PageSize >
bool splitted< PageSize >::kernel_free( void* addr )
{
	address_t free_addr( addr );
	fu_structure* cur_structure = 0;

#ifdef VERBOSE_ALLOC_FREE
	logger << message::MORE_VERBOSE << __func__
		<< ": freeing " << hex << free_addr.int_ptr << endl;
#endif //VERBOSE_ALLOC_FREE

	//find (calc) the address of the free/used structure
	{
		address_t struct_ptr( addr );
		struct_ptr.int_ptr -= sizeof( fu_structure );
		cur_structure = reinterpret_cast<fu_structure*>( struct_ptr.v_ptr );
	}

	if  (
			cur_structure && 
			( free_addr.int_ptr == cur_structure->allocated_memory.int_ptr )
		)
	{
		//found
		//check previous for merge
		if ( cur_structure->previous && cur_structure->previous->free )
		{
			//merge
			fu_structure* prev = cur_structure->previous;
			fu_structure* next = cur_structure->next;

			prev->size += sizeof( fu_structure );
			prev->size += cur_structure->size;

			//the previous next it currents next (cause current get killed)
			if ( prev )
			{
				prev->next = next;
			}

			//the currents nexts previous it currents previous
			if ( next )
			{
				next->previous = prev;
			}

			//forget about the old structure because it was killed due merging
			cur_structure = prev;
		}

		//check next for merge
		if ( cur_structure->next && cur_structure->next->free )
		{
			//merge
			fu_structure* next = cur_structure->next;

			cur_structure->size += sizeof( fu_structure );
			cur_structure->size += next->size;

			//the currents new next is his nexts next
			cur_structure->next = next->next;

			//get the new next
			next = cur_structure->next;

			//the nexts previous is the current
			if ( next )
			{
				next->previous = cur_structure;
			}
		}

		cur_structure->free = true;

		return true;
	}

	logger << message::MORE_VERBOSE << __func__
		<< ": Could not find the address" << endl;

	return false;
}

template< uint32_t PageSize >
void* splitted< PageSize >::page_alloc( bool kernel )
{
	address_t ret( 0 );

	ret = this->last_address;
	ret.int_ptr -= PageSize;
	uint64_t i( this->page_bm.length() - 1 );
	for (
			;
			i > 0;
			i--, ret.int_ptr -= PageSize
		)
	{
		if ( this->page_bm[i].free() )
		{
			this->page_bm[i].free( false );
			this->page_bm[i].kernel( kernel );
			break;
		}
	}

	if ( ret.int_ptr == 0 )
	{
		return ret.v_ptr;
	}
	else if ( this->cur_kernel_bound.int_ptr > ret.int_ptr )
	{
		this->page_bm[i].free( true );
		this->page_bm[i].kernel( false );
		return 0;
	}
	else if ( this->max_kernel_bound.int_ptr > ret.int_ptr )
	{
#ifdef VERBOSE_ALLOC_FREE
		logger << message::WARN << __func__
			<< ": giving old kernel space to userland. Clearing it!"
			<< endl;
#endif //VERBOSE_ALLOC_FREE

		uint32_t* it = reinterpret_cast<uint32_t*>(ret.v_ptr); //not optimal on 64bit archs
		for ( ; it < this->max_kernel_bound.v_ptr; it++ )
		{
			*it = 0;
		}

		//setting bound back
		this->max_kernel_bound = ret;
	}

	if ( this->cur_user_bound.int_ptr > ret.int_ptr )
	{
		this->cur_user_bound = ret;
	}

	return ret.v_ptr;
}

template< uint32_t PageSize >
bool splitted< PageSize >::page_free( void* addr )
{
	address_t free_addr( addr );
	uint64_t i( ( free_addr.int_ptr - this->first_address.int_ptr ) / PageSize );

#ifdef VERBOSE_ALLOC_FREE
	logger << message::MORE_VERBOSE << __func__
		<< ": free addr=" << hex << free_addr.int_ptr
		<< " entry=" << dec << i << endl;
#endif //VERBOSE_ALLOC_FREE

	if ( this->page_bm[i].free() )
	{
		//this page is not used so return error
#ifdef VERBOSE_ALLOC_FREE
		logger << message::MORE_VERBOSE << __func__
			<< ": Page to free was not used" << endl;
#endif //VERBOSE_ALLOC_FREE
		return false;
	}

	//mark as not used
	this->page_bm[i].free( true );

	// reset current user bound
	if ( addr == this->cur_user_bound.v_ptr )
	{
		// if we free the page at the border
		// we advance the bound til we found a page which is not free
		for ( uint64_t j = i; j < this->page_bm.length(); j++ )
		{
			if ( this->page_bm[i].free() )
			{
				// free (not used) so advance the bound
				this->cur_user_bound.int_ptr += PageSize;
			}
			else
			{
				// not free (used) so abort
				break;
			}
		}
#ifdef VERBOSE_ALLOC_FREE
		logger << message::MORE_VERBOSE << __func__
			<< ": User bound set to " << this->cur_user_bound.int_ptr << endl;
#endif //VERBOSE_ALLOC_FREE
	}

	//everything is okay
	return true;
}

template< uint32_t PageSize >
void *
splitted< PageSize >::vmalloc( uint32_t, process_type::hw_type&, void * )
{
	return 0;
}

} //namespace policies
} //namespace hlmemory
} //namespace iposix

#endif /* !_KERN_INCLUDE_MM_POLICIES_SPLITTED_HPP_ */
