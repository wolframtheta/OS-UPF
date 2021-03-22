
#include "myutils.h"

int n;
semaphore forks[5];

void * philosophize(void * params) {
  long id = (long)params;
  for (int k = 0; k < n; ++k) {
    usleep(100 + rand() % 100); // Thinking

    sem_wait(&forks[id]          );
    sem_wait(&forks[(id + 1) % 5]);

    usleep(100 + rand() % 100); // Eating

    sem_signal(&forks[(id + 1) % 5]);
    sem_signal(&forks[id]          );
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <no rounds>\n", argv[0]);
    return -1;
  }

  n = atoi(argv[1]);

  pthread_t tids[5];

  for (long i = 0; i < 5; ++i)
    sem_init(&forks[i], 1);

  for (long i = 0; i < 5; ++i)
    pthread_create(&tids[i], NULL, philosophize, (void *)i);

  for (long i = 0; i < 5; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

