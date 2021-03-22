#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for sleep function : waits for seconds
#include <time.h>    // for usleep : waits for microseconds
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#define N 10
int main(int argc, char *argv[]){
	int arr[2];
	arr[0]=0;
	arr[1]=0;
	int fd = open("data.dat",O_CREAT|O_RDWR,0644);
	write(fd,arr,sizeof(arr));
	close(fd);
	sem_unlink("named_mutex");
	for(int i = 0; i<10; i++){
		int pid = fork();
		if(pid==0){
			sem_t* named_mutex = sem_open("named_mutex",O_CREAT,0600,1);
			int fd = open("data.dat",O_CREAT|O_RDWR,0644,1);
			int index = i%2;
			for(int j = 0; j<10; j++){
				sem_wait(named_mutex);
				lseek(fd,0,SEEK_SET);
				read(fd,arr,sizeof(arr));
				arr[index]++;
				lseek(fd,0,SEEK_SET);
				write(fd,arr,sizeof(arr));
				sem_post(named_mutex);
			}
			close(fd);
			sem_close(named_mutex);
			_exit(0);
		}
	}
	while(wait(NULL)>0);
	fd = open("data.dat",O_CREAT|O_RDWR,0644,1);
	read(fd,arr,sizeof(arr));
	printf("array index 0: %d\n", arr[0]);
	printf("array index 1: %d\n", arr[1]);
	close(fd);
	return 0;
}
