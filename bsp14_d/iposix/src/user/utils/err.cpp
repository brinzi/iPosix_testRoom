#include "err.h"
#include <cstdlib>
#include <stdio.h>
#include <stdarg.h>

extern "C" void err( uint32_t exit_code, const char* format, ... )
{
	va_list ap;
	
	va_start( ap, format );
	vfprintf(stderr, format, ap);
	va_end( ap );

	if ( exit_code != 0 )
	{
		perror( 0 );
	}
	fprintf(stderr, "\n");

	exit( exit_code );
}

extern "C" void errx( uint32_t exit_code, const char* message )
{
	err( exit_code, "%s", message );
}
