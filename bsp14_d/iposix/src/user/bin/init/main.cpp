#include <unistd.h>
#include <sys/wait.h>
#include <halt.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Calls exit with success.
 */
int main()
{
	printf("Hello World!\n");

	printf("Init is running!\n");

	int pid = fork();

	if ( pid == 0 )
	{
		printf("Starting Shell...\n");
		execve( "/bin/shell", 0, 0 );
	}
	else
	{
		printf("Wait for Childprocess...\n");
		int status = 0;
		wait( &status );
	}

	printf("Calling halt...\n");
	halt();
	printf("Returned from halt...\n");

	while(1);

	printf("Calling exit...\n");
	exit(1);
	printf("Returned from exit...\n");
}
