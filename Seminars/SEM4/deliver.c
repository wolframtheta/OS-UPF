#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/wait.h>
#define N 10

/*
	Sergio Lopez 158574
	Delivery exercise 1 seminar 4

*/

int main(){
  int fd[N][2]; //pipe file descriptors
  int num; //number to write and read
  int count = 0; //total count
  
  for(int i = 0; i<N; i++){
  	pipe(fd[i]); //create pipe
	int pid = fork(); // create a process
	num = rand()%15; //asign random number
	if(pid==0){
		close(fd[i][0]); //close the reading part
		write(fd[i][1],&num,sizeof(int));
		printf("Child %d sends %d \n",i+1,num);
		_exit(1);
	}
  }
  for(int i = 0; i<N; i++){
 	close(fd[i][1]); //close the writting part
 	read(fd[i][0],&num,sizeof(int));
 	printf("Parent recives %d \n",num);
 	count += num;
 	
  }
  while (wait(NULL) > 0); //wait all process to finish
  printf("Total count: %d \n",count);
  return 0;
}
