#include <pthread.h> /* pthread_create */
#include "myutils.h"

#define NTHREADS 2

int sum = 0;

void * fsum(void * arg) {
  for (int i = 0; i < 1000; i++) {
    sum = sum + 1;
	  sleep(0);
  }
}

int main(void) {
  pthread_t tids[NTHREADS];

  for (int i = 0; i < NTHREADS; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(&tids[i], NULL, fsum, NULL);
  }

  for (int i = 0; i < NTHREADS; i++)
    pthread_join(tids[i], NULL);

  printf("Sum is %d\n", sum);

  return 0;
}
