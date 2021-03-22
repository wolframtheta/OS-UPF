
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>

int A[100];
pthread_mutex_t locks[100];

void * multiply(void * arg) {
  int * params = (int *)arg;
  for (int i = params[0]; i != params[2]; i += params[1]) {
    pthread_mutex_lock(&locks[i]);
    A[i] *= i;
    pthread_mutex_unlock(&locks[i]);
  }
  free(params);
}

int main(void) {
  pthread_t tids[2];
  for (long i = 0; i < 100; ++i) {
    A[i] = 1;
    pthread_mutex_init(&locks[i], NULL);
  }

  int * params = malloc(3 * sizeof(int));
  params[0] = 0;
  params[1] = 1;
  params[2] = 100;
  pthread_create(&tids[0], NULL, multiply, (void *)params);

  params = malloc(3 * sizeof(int));
  params[0] = 99;
  params[1] = -1;
  params[2] = -1;
  pthread_create(&tids[1], NULL, multiply, (void *)params);

  for (int i = 0; i < 2; ++i)
    pthread_join(tids[i], NULL);

  for (int i = 0; i < 100; ++i)
    printf("%d: %d\n", i, A[i]);

  return 0;
}

