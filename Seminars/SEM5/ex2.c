#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int fd[2];
	pipe(fd);
	int pid = fork();
	if(pid == 0){
		close(fd[0]); //close reading part
		dup2(fd[1],1); //replace std ouput with the pipe
		char* cmd[] = {"ls","-la",NULL};
		execvp(cmd[0],&cmd[0]);
		_exit(0);
	}
	
	while(wait(NULL)>0);
	close(fd[1]); // close writting part
	dup2(fd[0],0); //replace std input with the pipe read
	char *cmd[]={"wc","-l",NULL};
	execvp(cmd[0],&cmd[0]);
	
}
