#include "mm/memory_area.h"

namespace iposix {
namespace hlmemory {

memory_area::memory_area( void * low, void * high )
	: low( low ),
	  high( high )
{
}

bool
memory_area::operator< ( const memory_area& ma )
{
	return ( this->low < ma.low ) && ( this->high < ma.high ) && ( this->high < ma.low );
}

bool
memory_area::operator> ( const memory_area& ma )
{
	return ( this->low > ma.low ) && ( this->high > ma.high ) && ( this->low > ma.high );
}

uint32_t
memory_area::size()
{
	return reinterpret_cast< uint32_t >( high ) - reinterpret_cast< uint32_t >( low );
}

bool
memory_area::combine( const memory_area& ma )
{
	if( ma.low == this->high )
	{
		this->high = ma.high;
	}
	else if( this->low == ma.high )
	{
		this->low = ma.low;
	}
	else
	{
		return false;
	}

	return true;
}

}} //iposix::hlmemory
