#include <stdio.h>       /* printf, sprintf */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>      /* fork, _exit */
#include <sys/wait.h>    /* wait */
#include <sys/types.h>


int main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    int num;
    for (int i=0; i<10; i++) {
    	if(fork() == 0) {
    		close(fd[1]);
    		read(fd[0],&num,sizeof(int));
    		usleep(10000+rand()%10000);
    		printf("I'm process %d\n",num);
    		_exit(0);
    	}
    }
    for (int i=0; i<10; i++) {
        close(fd[0]); //close the reading part
 	write(fd[1],&i,sizeof(int));
    }
    while (wait(NULL) > 0);
    return 0;
    
}

