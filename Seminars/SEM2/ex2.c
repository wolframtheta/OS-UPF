#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "myutils.c"
#include <>
#define MAX 5

//exercise 6 from seminar 2
int main(int argc, char *argv[]){
	char* cmd[10];
	int icmd=0;
	char buff[80];
	int len = 0;
	int is_command = 1;
	int is_concurrent = 0;
	while(1){
		len = read_split(0,buff,80,&ch_end);
		if (is_command = 1){
			cmd[icmd] = malloc(len);
			strcopy(cmd[icmd],buff);
			icmd++;
			is_command=0;
		}else{
			if(buff[0] == '&'){
				is_concurrent = 1;
			}
			cmd[icmd] = malloc(len);
			strcpy(cmd[icmd],buff);
			icmd++;
		}
		if (ch_end == '\n') break;
	} if (ch_end == '\n'){
		icmd=0;
		is_command=1;
		cmd[icmd] = NULL;
		int pid = fork();
		if (pid == 0){
			execvp(cmd[0],&cmd[0]);
			_exit(0);
		}
		if(is_concurrent){
			write(1,"done\n",strlen("done\n"));
			is_concurrent = 0;
		}else{
			wait(NULL);
		}
	}
}
