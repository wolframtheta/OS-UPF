
#include <pthread.h>
#include <stdio.h>

int A[100];
pthread_mutex_t locks[100];

void * multiply(void * arg) {
  long i = (long)arg;
  pthread_mutex_lock(&locks[i]);
  A[i] *= i;
  pthread_mutex_unlock(&locks[i]);
}

int main(void) {
  pthread_t tids[100][2];

  for (long i = 0; i < 100; ++i) {
    A[i] = 1;
    pthread_mutex_init(&locks[i], NULL);
    pthread_create(&tids[i][0], NULL, multiply, (void *)i);
    pthread_create(&tids[i][1], NULL, multiply, (void *)i);
  }

  for (int i = 0; i < 100; ++i) {
    pthread_join(tids[i][0], NULL);
    pthread_join(tids[i][1], NULL);
    printf("%d: %d\n", i, A[i]);
  }

  return 0;
}

