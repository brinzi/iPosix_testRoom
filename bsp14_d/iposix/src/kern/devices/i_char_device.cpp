#include "devices/i_char_device.h"

namespace iposix {
namespace arch {

i_char_device::i_char_device( const ::std::string& name )
	: name( name )
{ }

uint32_t i_char_device::read(
				uint64_t base_address, 
				void* buffer, 
				uint32_t length)
{
	if ( base_address != 0 )
	{
		return 0;
	}
	else
	{
		return this->read( buffer, length );
	}
}

uint32_t i_char_device::write(
		uint64_t base_address, 
		const void* buffer, 
		uint32_t length)
{
	if ( base_address != 0 )
	{
		return 0;
	}
	else
	{
		return this->write( buffer, length );
	}
}

} //namespace arch
} //namespace iposix
