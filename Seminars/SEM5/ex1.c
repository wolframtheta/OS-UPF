#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	char *cmds[][5]={{"ps",NULL}, {"ls","−la",NULL}, {"cat","data.txt",NULL}};
	
	for (int i = 0; i<3; i++){
		int pid = fork();
		if(pid==0){
			execvp(cmds[i][0],cmds[i]);
			_exit(0);
		}
	}
	while(wait(NULL) > 0);
	return 0;
}
