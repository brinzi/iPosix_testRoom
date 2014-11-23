#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Calls exit with success.
 */
int main()
{
	printf("Hello World!\n");

	int pid = fork();
	
	if ( pid == 0 )
	{
		while ( true  )
		{
			printf("Te");
		}
	}
	else
	{
		while ( true )
		{
			printf("Iubesc");
		}
	}

	printf("Calling exit...\n");
	exit(1);
	printf("Returned from exit...\n");
}
