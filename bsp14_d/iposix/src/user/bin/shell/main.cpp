#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Calls exit with success.
 */
int main()
{
	printf("Hello World!\n");

	printf("Shell is running!\n");

	printf("Calling exit...\n");
	exit(0);
}
