#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <err.h>

/**
 * Create a given directory.
 */
int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		errx( 1, "Usage: mkdir <DIRECTORY>" );
	}

	if ( mkdir( argv[1], 0775 ) )
	{
		err( 1, "%s", argv[1] );
	}

	exit(0);
}
