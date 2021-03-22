#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for sleep function : waits for seconds
#include <time.h>    // for usleep : waits for microseconds

#define NTHREADS 10

struct thread_info{
	int id;
	int a,b;
	char* message; 
}thread_info;

struct thread_info thread_data[NTHREADS];

void* fthread(void * arg) {
  struct thread_info *arguments = (struct thread_info*) arg;
  int id = arguments->id;
  int a = arguments->a;
  int b = arguments->b;
  char* msg = arguments->message;
  printf("thread with id:%d has this values: %d,%d and a message: %s\n", id,a,b,msg);
  return NULL;
}

int main(int argc, char *argv[]){
	pthread_t tids[NTHREADS];
	for (int i = 0; i<NTHREADS; i++){
		thread_data[i].id = i;
		thread_data[i].a = rand()%10;
		thread_data[i].b = rand()%30;
		thread_data[i].message = "hello";
		pthread_create(&tids[i],NULL,fthread,(void*)&thread_data[i]);
	}
	for (int i = 0; i<NTHREADS; i++){pthread_join(tids[i],NULL);}

}
