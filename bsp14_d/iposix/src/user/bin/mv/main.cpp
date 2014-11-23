#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/**
 * Removes a given set of files.
 */
int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		errx( 1, "Usage: mv <OLDFILE> <NEWFILE>" );
	}

	if ( rename( argv[1], argv[2] ) )
	{
		err( 1, "%s or %s", argv[1], argv[2] );
	}

	exit(0);
}
