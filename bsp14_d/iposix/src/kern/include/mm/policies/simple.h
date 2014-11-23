#include <stdint.h>
#include "mm/memory_area.h"
#include "mm/policies/straight.h"
#include "utils/singleton.hpp"
#include "scheduler/i_process.h"

namespace iposix {
namespace hlmemory {
namespace policies {

using scheduler::process_type;

template< uint32_t Unit = 0 >
struct simple
	: private straight< Unit >,
	  public ::iposix::utils::Singleton< simple< Unit > >
{
	friend class ::iposix::utils::Singleton< simple< Unit > >;

   private:
	typedef straight< Unit > straight_type;

	struct alloc
	{
		void * addr;
		uint32_t size;
	};

	enum constants
	{
		allocs_size = 1024
	};

   public:
	alloc allocs[allocs_size];
	alloc * first_free_alloc;

	void * malloc( uint32_t size, bool kernel );
	bool free( void * addr, bool kernel  );
	template< typename T >
	void set_memory_areas( const T& mem );

	void * vmalloc( uint32_t, void * );

   private:

   	simple()
		: straight_type(),
		  first_free_alloc( &allocs[0] )
	{
		for( uint32_t i = 0; i < allocs_size; ++i )
		{
			allocs[i].addr = 0;
		}
	}

	void merge_areas();
};

template< uint32_t Unit >
void *
simple< Unit >::malloc( uint32_t size, bool kernel )
{
	if( this->first_free_alloc == 0 ) //no allocs available
	{
		return 0;
	}

	void * addr = straight_type::malloc( size, kernel );
	
	if( addr == 0 )
	{
		return 0;
	}

	this->first_free_alloc->addr = addr;
	this->first_free_alloc->size = size;

	++this->first_free_alloc;

	while( this->first_free_alloc < &this->allocs[allocs_size] )
	{
		if( this->first_free_alloc->addr == 0 )
		{
			break;
		}

		++this->first_free_alloc;
	}

	if( this->first_free_alloc == &this->allocs[allocs_size] )
	{
		this->first_free_alloc = 0;
	}
	
	return addr;
}

template< uint32_t Unit >
bool
simple< Unit >::free( void * area, bool /*kernel*/ )
{
	if( area == 0 )
	{
		return true;
	}

	alloc * a;	

	for( a = &this->allocs[0]; a < &this->allocs[allocs_size]; ++a )
	{
		if( a->addr == area )
			break;
	}

	if( a == &this->allocs[allocs_size] )
	{
		return false;
	}

	memory_area ma( area, reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( area ) + a->size ) );

	if( this->area_count != this->max_areas )
	{
		this->areas[this->area_count++] = ma;
		this->merge_areas();
	}

	a->addr = 0;
	if( this->first_free_alloc > a )
	{
		this->first_free_alloc = a;
	}

	return true;
}

template< uint32_t Unit >
void
simple< Unit >::merge_areas()
{
	uint32_t new_count( 0 );

	for( memory_area * area = &this->areas[0]; area < &this->areas[this->area_count]; ++area )
	{
		bool combined( false );
		for( memory_area * it = area + 1; it < &this->areas[this->area_count]; ++it )
		{
			if( it->combine( *area ) )
			{
				combined = true;
				break;
			}
		}

		if( !combined )
		{
			this->areas[new_count++] = *area;
		}
	}

	this->area_count = new_count;
}

template< uint32_t Unit >
template< typename T >
void
simple< Unit >::set_memory_areas( const T& mem )
{
	straight_type::set_memory_areas( mem );
}

template< uint32_t Unit >
void *
simple< Unit >::vmalloc( uint32_t, process_type::hw_type&, void * )
{
	return 0;
}

}}} //iposix::hlmemory::policies
