#define __rtems__ //this is a hack
#include <unistd.h>
#undef __rtems__
#include <stdlib.h>

/**
 * Calls sync and exits with success.
 */
int main()
{
	sync();
	exit(0);
}
