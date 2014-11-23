#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <limits.h>

/**
 * Removes a given set of files.
 */
int main( int, char** )
{
	char buffer[MAXPATHLEN + 1];
	char* buffer_ptr;

	buffer_ptr = getcwd( &buffer[0], sizeof( buffer ) );

	if ( ! buffer_ptr )
	{
		err( 1, "." );
	}

	printf( "%s\n", &buffer[0] );

	exit( 0 );
}
