#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for sleep function : waits for seconds
#include <time.h>    // for usleep : waits for microseconds
#include <sys/wait.h>
#define N 10

int main(int argc, char *argv[]){
	int fd[2];
	pipe(fd);
	for (int i = 0; i<N; i++){
		int pid = fork();
		if(pid == 0) {
			int id;
			close(fd[1]); //close write
			read(fd[0],&id,sizeof(int));
			printf("%d\n",id);
			close(fd[0]);
			_exit(0);
		}else{
			write(fd[1],&i,sizeof(int));
		}

	}
	while(wait(NULL)>0);
	return 0;
	
}
