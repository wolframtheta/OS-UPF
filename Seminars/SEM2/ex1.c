#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "myutils.c"
#define MAX 5

//exercise 4: reading input from terminal
int main(int argc, char *argv[]){
	startTimer(0);
	char* cmd[] = {"ps",NULL};
	int num;
	sscanf(argv[1],"%d",&num);
	for (int i = 0; i < num; i++){
		int pid = fork();
		if (pid == 0){
			execvp(cmd[0],&cmd[0]);
			_exit(0);
		}
	}
	while (wait(NULL)>0);
	printf("%lu \n",endTimer(0));
}
