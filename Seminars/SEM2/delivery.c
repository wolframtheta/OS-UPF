#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

//Exercise of threads from seminar 2 Sergio Lopez cobo 158574

/*
When we're using two or more threads concurrently, they may access and update the same data,we call this part the critical section of the thread.If we don't syncronize it, various threads can interact with this data at the same time, making errors in the program. Since this problems are in the code and not in the compilation, the program may compile and those errors can be hard to detect. 
In order to avoid this, we use a synchronize mecanism,called locks. So when a thread acces to a critical section, we apply this mecanism to control that no other threads acces to this part until the current thread finishes. So, other threads must wait.

In our case, since we want all threads to use the lock, we should initialize it in the main.Then we apply it in the critical section, that is in the thread function, in my case inside fthread. We don't need more than 1 lock, since all threads gonna use it in the thread function.

*/

int A[100];
void* fthread(void* param){
	int i = *(int*) param; //cast i to an int
	A[i] = i*i;
	free(param); //free allocation memory to avoid leaks
	pthread_exit(0); 
}

int main(int argc, char *argv[]){
	pthread_t tids [200]; //space for 200 threads
	//creating 100 threads
	for (int i = 0; i<100; i++){
		A[i]=1;
		int* i_copy = malloc(sizeof(int));
		*i_copy = i;
		pthread_create(&tids[i],NULL,fthread, i_copy);
	}
	//creating the other 100 threads
	for (int i = 100; i<200; i++){
		int* i_copy = malloc(sizeof(int));
		*i_copy = i;
		pthread_create(&tids[i],NULL,fthread, i_copy);
	}
	//waiting until all threads finish
	for (int i = 0; i<200; i++){
		pthread_join(tids[i],NULL);
	}
	for (int i = 0; i<100; i++){
		printf("%d ",A[i]);
	}
}
