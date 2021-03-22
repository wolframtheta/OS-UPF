#include <pthread.h> /* pthread_create */
#include "myutils.h"

void * fsleep(void * arg) {
  for (int i = 0; i < 10; i++)
    sleep(1);
}

int main(void) {
  pthread_t tids[2];

  startTimer(0);
  for (int i = 0; i < 2; i++) {
    printf("Creating thread %d\n", i);
    pthread_create(&tids[i], NULL, fsleep, NULL);
  }

  for (int i = 0; i < 2; i++)
    pthread_join(tids[i], NULL);

  printf("Time spent: %ld\n", endTimer(0));

  return 0;
}
