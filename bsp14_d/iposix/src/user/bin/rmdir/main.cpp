#include <unistd.h>
#include <stdlib.h>
#include <err.h>

/**
 * Removes a given directory.
 */
int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		errx( 1, "Usage: rmdir <DIRECTORY>" );
	}

	if ( rmdir( argv[1] ) )
	{
		err( 1, "Could not remove %s", argv[1] );
	}

	exit(0);
}
