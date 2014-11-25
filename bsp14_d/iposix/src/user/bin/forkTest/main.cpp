#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
	int pid=fork();
	
	if(pid==0)
		printf("i am paratent\n");
		printf("i am child\n");
	
		exit(1);
}
