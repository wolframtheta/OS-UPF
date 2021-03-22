
#include "myutils.h"

#define N 10  // threads
#define I 100 // reader iterations

int R, WR, W, WW;
pthread_mutex_t mutex;      /* mutex lock      */
pthread_cond_t  read_cond;  /* read condition  */
pthread_cond_t  write_cond; /* write condition */

void * readerThread(void * params) {
  for (int k = 0; k < N; ++k) {
    pthread_mutex_lock(&mutex);
    while (W + WW > 0) {
      WR++;
      pthread_cond_wait(&read_cond, &mutex);
      WR--;
    }
    R++;
    pthread_mutex_unlock(&mutex);

    usleep(50); // Access database

    pthread_mutex_lock(&mutex);
    R--;
    if (R == 0 && WW > 0)
      pthread_cond_signal(&write_cond);
    pthread_mutex_unlock(&mutex);

    usleep(50); // Consume data
  }
  printf("Reader %ld finished!\n", (long)params);
}

void * writerThread(void * params) {
  usleep(50); // Produce data

  pthread_mutex_lock(&mutex);
  while (W + R > 0) {
    WW++;
    pthread_cond_wait(&write_cond, &mutex);
    WW--;
  }
  W++;
  pthread_mutex_unlock(&mutex);

  usleep(50); // Access database
  printf("Writer %ld finished!\n", (long)params);

  pthread_mutex_lock(&mutex);
  W--;
  if (WW > 0) pthread_cond_signal(&write_cond);
  else pthread_cond_broadcast(&read_cond);
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {
  pthread_t tids[2 * N];

  R = WR = W = WW = 0;
  pthread_mutex_init(&mutex     , NULL);
  pthread_cond_init (&read_cond , NULL);
  pthread_cond_init (&write_cond, NULL);

  for (long i = 0; i < N; ++i) {
    pthread_create(&tids[i]    , NULL, readerThread, (void *)i);
    pthread_create(&tids[N + i], NULL, writerThread, (void *)i);
  }

  for (long i = 0; i < 2 * N; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

