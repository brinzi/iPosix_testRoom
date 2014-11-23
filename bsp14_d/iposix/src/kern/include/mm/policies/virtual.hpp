#ifndef _KERN_INCLUDE_MM_POLICIES_H_
#define _KERN_INCLUDE_MM_POLICIES_H_

#include "utils/singleton.hpp"
#include "scheduler/i_process.h"

namespace iposix {
namespace hlmemory {
namespace policies {

using ::iposix::scheduler::process_type;

template< template< uint32_t > class Memory >
struct virtual_mm
	: ::iposix::utils::Singleton< virtual_mm< Memory > >
{
	friend class ::iposix::utils::Singleton< virtual_mm< Memory > >;

	void * malloc( uint32_t size, bool kernel, bool aligned, void * base = 0, process_type::hw_type& proc = *scheduler::current_process() );
	void * vmalloc( uint32_t size, process_type::hw_type& proc, void * base );
	bool free( void * addr, bool kernel );

	template< typename T >
	void set_memory_areas( const T& );

	bool initialized()
	{
		return mem.initialized();
	}

   private:
	virtual_mm();

	Memory< process_type::page_size >& mem;
};

template< template< uint32_t > class Memory >
virtual_mm< Memory >::virtual_mm()
	: mem( ::iposix::utils::Singleton< Memory< process_type::page_size > >::instance() )
{ }

template< template< uint32_t > class Memory >
void *
virtual_mm< Memory >::malloc( uint32_t size, bool kernel, bool aligned, void * base, process_type::hw_type& proc )
{
	//no support for swapping at the moment
	if( kernel )
	{
		return mem.malloc( size, true, aligned );
	}
	else
	{
		uint32_t pages = size / process_type::page_size;
		uint32_t mod = size % process_type::page_size;
		if( mod )
		{
			++pages;
		}

		vector< void * > addresses;
		addresses.reserve( pages );

		for( uint32_t i = 0; i < pages; ++i )
		{
			void * addr = mem.malloc( process_type::page_size, false, false ); //aligned nur fuer den kernel moeglich

			if( !addr )
			{
				auto it = addresses.begin();
				auto it_end = addresses.end();

				for( ; it != it_end; ++it )
				{
					mem.free( *it, false );
				}

				return 0;
			}

			addresses.push_back( addr );
		}

		return proc.add_virtual_memory( addresses, base );
	}
}

template< template< uint32_t > class Memory >
void *
virtual_mm< Memory >::vmalloc( uint32_t size, process_type::hw_type& proc, void * base )
{
	if( base == 0 )
	{
		return 0;
	}

	return this->malloc( size, false, false, base, proc );
}

template< template< uint32_t > class Memory >
bool
virtual_mm< Memory >::free( void * addr, bool kernel )
{
	if( kernel )
	{
		return mem.free( addr, kernel );
	}
	else
	{
		process_type* current = scheduler::current_process();

		if ( !current )
		{
			return false;
		}

		//TODO wenn mehr als eine Seite angefordert wurde, wird hier nur die erste wieder freigegeben

		void * real = current->get_phys_address( addr );
		if( !real )
		{
			return false;
		}

		if( !mem.free( real, false ) )
		{
			return false;
		}

		return current->remove_virtual_memory( addr );
	}
}

template< template< uint32_t > class Memory >
template< typename T >
void
virtual_mm< Memory >::set_memory_areas( const T& m )
{
	this->mem.set_memory_areas( m );
}

}}} //iposix::hlmemory::policies

#endif /* !_KERN_INCLUDE_MM_POLICIES_H_ */
