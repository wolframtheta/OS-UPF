
#include "myutils.h"

#define N 10  // threads
#define I 100 // reader iterations

int R;
semaphore database;
semaphore mutex;

void * readerThread(void * params) {
  for (int k = 0; k < N; ++k) {
    sem_wait(&mutex);
    R++;
    if (R == 1) sem_wait(&database);
    sem_signal(&mutex);

    usleep(50); // Access database

    sem_wait(&mutex);
    R--;
    if (R == 0) sem_signal(&database);
    sem_signal(&mutex);

    usleep(50); // Consume data
  }
  printf("Reader %ld finished!\n", (long)params);
}

void * writerThread(void * params) {
  usleep(50); // Produce data

  sem_wait(&database);

  usleep(50); // Access database
  printf("Writer %ld finished!\n", (long)params);

  sem_signal(&database);
}

int main(int argc, char *argv[]) {
  pthread_t tids[2 * N];

  R = 0;
  sem_init(&database, 1);
  sem_init(&mutex   , 1);

  for (long i = 0; i < N; ++i) {
    pthread_create(&tids[i]    , NULL, readerThread, (void *)i);
    pthread_create(&tids[N + i], NULL, writerThread, (void *)i);
  }

  for (long i = 0; i < 2 * N; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

