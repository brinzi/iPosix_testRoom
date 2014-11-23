#include <unistd.h>
#include <stdlib.h>
#include <err.h>

/**
 * Removes a given set of files.
 */
int main( int argc, char* argv[] )
{
	if ( argc < 2 )
	{
		errx( 1, "Usage: rm <FILES...>" );
	}

	for ( int i = 1; i < argc; ++i )
	{
		if ( unlink( argv[i] ) )
		{
			err( 1, "%s", argv[i] );
		}
	}

	exit(0);
}
