#include "tests/tests.h"
#include "logger/logger.h"

#include <stdint.h>
#include <vector>
#include <list>

namespace iposix {
namespace tests {

bool memory_test()
{
	logger.print( "Testing std::vector and std::list ... " );
	{
		std::vector< uint64_t > test_vector;
		test_vector.reserve( 1000 );
		std::list< uint64_t > test_list;
		uint64_t test_array[1000];
		{
			uint64_t * it = &test_array[0];
			uint64_t * const it_end = it + sizeof( test_array ) / sizeof( decltype( test_array[0] ) );
			uint64_t value( 0 );

			for( ; it < it_end; ++it )
			{
				*it = value;
				test_vector.push_back( value );
				test_list.push_back( value );
				value += 0x123F;
			}
		}

		{
			auto it = test_vector.begin();
			const auto it_end = test_vector.end();
			auto it_list = test_list.begin();
			const uint64_t * it_array = &test_array[0];
			bool failed( false );

			for( ; it != it_end; ++it )
			{
				if( *it != *it_array || *it != *it_list )
				{
					failed = true;
					break;
				}
				++it_array;
				++it_list;
			}

			if( failed )
			{
				logger.print( "failed", endl );
				return false;
			}
			else
			{
				logger.print( "ok", endl );
				return true;
			}
		}
	}
}

} //namespace tests
} //namespace iposix
