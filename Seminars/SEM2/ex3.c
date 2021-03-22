#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

//Exercise of threads from seminar 2
int A[100];
void* fthread(void* param){
	int i = *(int*) param;
	A[i] = i*i;
	free(param);
	pthread_exit(0);
}

int main(int argc, char *argv[]){
	pthread_t tids [100];
	for (int i = 0; i<100; i++){
		int* i_copy = malloc(sizeof(int));
		*i_copy = i;
		pthread_create(&tids[i],NULL,fthread, i_copy);
	}
	for (int i = 0; i<100; i++){
		pthread_join(tids[i],NULL);
	}
}
