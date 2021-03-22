
#include "myutils.h"

#define N 10  // threads
#define I 100 // reader iterations

int R;
semaphore database;
semaphore mutex;
semaphore swap;

void * readerThread(void * params) {
  for (int k = 0; k < N; ++k) {
    sem_wait(&swap );
    sem_wait(&mutex);
    R++;
    if (R == 1) sem_wait(&database);
    sem_signal(&mutex);
    sem_signal(&swap );

    usleep(10); // Access database

    sem_wait(&mutex);
    R--;
    if (R == 0) sem_signal(&database);
    sem_signal(&mutex);

    usleep(10); // Consume data
  }
  printf("Reader %ld finished!\n", (long)params);
}

void * writerThread(void * params) {
  usleep(1000); // Produce data

  sem_wait(&swap    );
  sem_wait(&database);

  usleep(10); // Access database
  printf("Writer %ld finished!\n", (long)params);

  sem_signal(&database);
  sem_signal(&swap    );
}

int main(int argc, char *argv[]) {
  pthread_t tids[2 * N];

  R = 0;
  sem_init(&database, 1);
  sem_init(&mutex   , 1);
  sem_init(&swap    , 1);

  for (long i = 0; i < N; ++i) {
    pthread_create(&tids[i]    , NULL, readerThread, (void *)i);
    pthread_create(&tids[N + i], NULL, writerThread, (void *)i);
  }

  for (long i = 0; i < 2 * N; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

