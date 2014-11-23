#include "tests/tests.h"
#include "logger/logger.h"

#include <exception>

namespace iposix {
namespace tests {

typedef bool (*test_fct_ptr)();

inline void test( test_fct_ptr fct_ptr, const char* name )
{
	logger << message::VERBOSE << "Testing: " << name;
	try
	{
	if ( fct_ptr() )
	{
		logger << message::VERBOSE << " successed" << endl;
	}
	else
	{
		logger << message::VERBOSE << " failed" << endl;
	}
	}
	catch ( ::std::exception& e )
	{
		logger << message::VERBOSE << " failed with "
			<< e.what() << endl;
	}
}

void all_tests()
{
	#define tst( function ) \
			test( function, #function )

	tst( memory_test );
	tst( mount_test );
	tst( i_fs_test );
	tst( elf_loader_test );
	tst( process_test );
}

} //namespace tests
} //namespace iposix
