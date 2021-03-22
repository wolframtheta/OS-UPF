#include <malloc.h>  /* malloc, free   */
#include <pthread.h> /* pthread_create */
#include "myutils.h"

#define N 10
pthread_t tids[N];

void * myThread(void * param) {	/* thread function */
  int * i_ptr = (int *)param; 
  pthread_t tid = pthread_self();
  printf("Thread #%d with ID %lu\n", *i_ptr, tid);
  sleep(1);
}

int main(void) {
  int i;
  startTimer(0);

  // better to allocate thread arguments on heap!
  int * pints = malloc(N * sizeof(int));

  for (i = 0; i < N; i++) {
    pints[i] = i;
    pthread_create(&tids[i], NULL, myThread, &pints[i]);
  }

  printf("Parent will wait\n");
  for (i = 0; i < N; i++)
    pthread_join(tids[i], NULL);

  free(pints);

  long t = endTimer(0);
  printf("Parent ended and spent %ld ms\n", t);

  return 0;
}

