#ifndef _KERN_INCLUDE_MM_KMALLOC_H_
#define _KERN_INCLUDE_MM_KMALLOC_H_

#include "iposix/iposix_kernel.h"
#include "mm/policies/splitted.hpp"
#include "mm/policies/virtual.hpp"
#include "scheduler/i_process.h"
#include "logger/logger.h"

#define STANDARD_STRATEGY policies::virtual_mm< policies::splitted >

namespace iposix {
namespace hlmemory {

using scheduler::process_type;

template< typename Strategy = STANDARD_STRATEGY >
inline static void *
kmalloc( uint32_t size, bool kernel = true, bool aligned = false )
{
	Strategy& s = ::iposix::utils::Singleton< Strategy >::instance();

	if( !s.initialized() )
	{
		s.set_memory_areas( ::iposix::utils::Singleton< typename ::iposix::kernel::memory_type >::instance() );
	}

	void* ret = s.malloc( size, kernel, aligned );

	if ( !ret )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Returning null-pointer" << endl;
	}

	return ret;
}

template< typename Strategy = STANDARD_STRATEGY >
inline static void *
vmalloc( uint32_t size, process_type::hw_type& proc, void * base )
{
	Strategy& s = ::iposix::utils::Singleton< Strategy >::instance();

	return s.vmalloc( size, proc, base );
}

template< typename Strategy = STANDARD_STRATEGY >
inline static bool
kfree( void * addr, bool kernel )
{
	Strategy& s = ::iposix::utils::Singleton< Strategy >::instance();

	bool ret = s.free( addr, kernel );

	if ( !ret )
	{
		logger << message::MORE_VERBOSE << __func__
			<< ": Illegal or errornous kfree" << endl;
	}

	return ret;
}

}} //iposix::hlmemory

#undef STANDARD_STRATEGY

#endif /* !_KERN_INCLUDE_MM_KMALLOC_H_ */
