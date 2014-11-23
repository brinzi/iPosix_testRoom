#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	fork();
	printf("Hi\n");
	exit(1);
}
