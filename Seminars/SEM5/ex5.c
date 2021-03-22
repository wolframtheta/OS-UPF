#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for sleep function : waits for seconds
#include <time.h>    // for usleep : waits for microseconds
#include <pthread.h>

#define NTHREADS 10
pthread_mutex_t locks[2];
int arr[2];

void* fthread(void* arg){
	int id = *(int*)arg;
	int index = id%2; //return 0 or 1;
	for(int i = 0; i<1000; i++){
		pthread_mutex_lock(&locks[index]);
		arr[index]++;
		pthread_mutex_unlock(&locks[index]);
	}
	free(arg);
	return NULL;
}

int main(){
	pthread_t tids[NTHREADS];
	for (int i = 0; i<2; i++){pthread_mutex_init(&locks[i],NULL);}
	for (int i = 0; i<NTHREADS; i++){
		int *m = malloc(sizeof(int));
		*m = i;
		pthread_create(&tids[i],NULL,fthread,m);

	}
	for (int i = 0; i<NTHREADS; i++){pthread_join(tids[i],NULL);}
	printf("array index 0: %d\n", arr[0]);
	printf("array index 1: %d\n", arr[1]);
	
	}

