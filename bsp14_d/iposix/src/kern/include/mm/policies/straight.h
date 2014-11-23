#include <stdint.h>
#include "mm/memory_area.h"
#include "utils/singleton.hpp"
#include "scheduler/i_process.h"

namespace iposix {
namespace hlmemory {
namespace policies {

using scheduler::process_type;

template< uint32_t Unit = 0 >
struct straight
	: public ::iposix::utils::Singleton< straight< Unit > >
{
	friend class ::iposix::utils::Singleton< straight< Unit > >;

	enum constants { max_areas = 10 };
	memory_area areas[max_areas];
	uint32_t area_count;

	void * malloc( uint32_t size, bool kernel );
	bool free( void * addr, bool kernel  );
	template< typename T >
	void set_memory_areas( const T& mem );

	void * vmalloc( uint32_t, process_type::hw_type&, void * );

   	straight()
		: area_count( 0 )
	{ }
};

template< uint32_t Unit >
void *
straight< Unit >::malloc( uint32_t size, bool /*kernel*/ )
{
	if( this->area_count == 0 )
	{
		return 0;
	}

	for( memory_area * ma = &this->areas[0]; ma <= &this->areas[this->area_count - 1]; ++ma )
	{
		if( ma->size() >= size ) //fits -> use it
		{
			void * ret = ma->low;
			ma->low = reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( ma->low ) + size );

			if( Unit != 0 )
			{
				auto unit = Unit;
				auto modulo = reinterpret_cast< uintptr_t >( ma->low ) % unit;
				ma->low = reinterpret_cast< void * >( reinterpret_cast< uintptr_t >( ma->low ) + ( Unit - modulo ) ); //mlow auf die nächste Unit hochsetzen
			}

			return ret;
		}
	}
	
	return 0;
}

template< uint32_t Unit >
bool
straight< Unit >::free( void * /*area*/, bool /*kernel*/ )
{
	return true;
}

template< uint32_t Unit >
template< typename T >
void
straight< Unit >::set_memory_areas( const T& mem )
{
	this->area_count = mem.get_memory_areas( &this->areas[0], this->max_areas );

	if( Unit == 0 )
	{
		return;
	}
	else
	{
		for( memory_area * m = &this->areas[0]; m < &this->areas[0] + this->area_count; ++m )
		{
			uintptr_t mlow = reinterpret_cast< uintptr_t >( m->low );
			uintptr_t mhigh = reinterpret_cast< uintptr_t >( m->high );
			uintptr_t unit = Unit;

			uintptr_t modulo = mlow % unit;

			mlow = mlow + ( Unit - modulo ); //mlow auf die nächste Unit hochsetzen
			mhigh = mhigh - modulo; //mhigh auf die nächste Unit setzen

			if( mhigh < mlow )
			{
				mhigh = mlow;
			}
		}
	}
}

template< uint32_t Unit >
void *
straight< Unit >::vmalloc( uint32_t, process_type::hw_type&, void * )
{
	return 0;
}

}}} //iposix::hlmemory::policies
