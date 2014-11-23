#include "devices/i_block_device.h"

#include <cstring>

namespace iposix {
namespace arch {

i_block_device::i_block_device( const ::std::string& name )
	: name( name )
{ }

int32_t i_block_device::read(
				uint64_t base_address, 
				void* buffer, 
				uint32_t length)
{
	uint32_t block_size( this->block_size() );

	uint8_t* buf( reinterpret_cast<uint8_t*>( buffer ) );
	uint8_t* tmp( new uint8_t[block_size] );

	if ( tmp == 0 )
	{
		return -1;
	}

	uint64_t base_block( base_address / block_size );
	uint64_t skip_bytes( base_address % block_size );
	uint32_t remain( length );
	uint32_t position( 0 );
	bool success( true );

	//getting the head
	if ( skip_bytes > 0 )
	{
		uint32_t max_len( block_size - skip_bytes );
		uint32_t len( remain > max_len ? max_len : remain );

		int32_t transfered = this->read_block( base_block, &tmp[0], 1 );

		success &= ( transfered != -1 );

		::std::memcpy( &buf[position], &tmp[skip_bytes], len );

		position += len;
		remain -= len;
		skip_bytes = 0;
		base_block++;

	}

	//getting the main part
	while ( ( remain > block_size ) && success )
	{
		//cut off the tail
		uint32_t blocks( remain / block_size );

		int32_t transfered = this->read_block( base_block, &buf[position], blocks );
		success &= ( transfered != -1 );

		if ( success )
		{
			position += transfered * block_size;
			remain -= transfered * block_size;
			base_block += transfered;
		}
	}

	//getting the tail
	if ( ( remain > 0 ) && success )
	{
		int32_t transfered = this->read_block( base_block, &tmp[0], 1 );

		success &= ( transfered != -1 );

		memcpy( &buf[position], &tmp[skip_bytes], remain );
	}

	delete[] tmp;

	return success ? length : -1;
}

int32_t i_block_device::write(
				uint64_t base_address, 
				const void* buffer, 
				uint32_t length)
{
	uint32_t block_size( this->block_size() );

	const uint8_t* buf( reinterpret_cast<const uint8_t*>( buffer ) );
	uint8_t* tmp( new uint8_t[block_size] );
	uint64_t base_block( base_address / block_size );
	uint64_t skip_bytes( base_address % block_size );
	uint32_t remain( length );
	uint32_t position( 0 );
	bool success( true );

	//getting the head
	if ( skip_bytes > 0 )
	{
		uint32_t max_len( block_size - skip_bytes );
		uint32_t len( remain > max_len ? max_len : remain );


		//read content
		int32_t transfered = this->read_block( base_block, &tmp[0], 1 );
		success &= ( transfered != -1 );

		//modify content
		memcpy( &tmp[skip_bytes], &buf[position], len );

		//write content
		transfered = this->write_block( base_block, &tmp[0], 1 );
		success &= ( transfered != -1 );

		position += len;
		remain -= len;
		skip_bytes = 0;
		base_block++;

	}

	//getting the main part
	while ( ( remain > block_size ) && success )
	{
		//cut off the tail
		uint32_t blocks( remain / block_size );

		int32_t transfered = this->write_block( base_block, &buf[position], blocks );
		success &= ( transfered != -1 );

		if ( success )
		{
			position += transfered * block_size;
			remain -= transfered * block_size;
			base_block += transfered;
		}
	}

	//getting the tail
	if ( ( remain > 0 ) && success )
	{
		//read content
		int32_t transfered = this->read_block( base_block, &tmp[0], 1 );
		success &= ( transfered != -1 );

		//modify content
		memcpy( &tmp[0], &buf[position], remain );

		//write content
		transfered = this->write_block( base_block, &tmp[0], 1 );
		success &= ( transfered != -1 );
	}

	delete[] tmp;

	return success ? length : -1;
}

} //namespace arch
} //namespace iposix
