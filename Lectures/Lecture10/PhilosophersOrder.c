
#include "myutils.h"

int n;
semaphore forks[5];

void * philosophize(void * params) {
  long id = (long)params;
  for (int k = 0; k < n; ++k) {
    usleep(100 + rand() % 100); // Thinking

    if (id < 4) {
      sem_wait(&forks[id]    );
      sem_wait(&forks[id + 1]);
    }
    else {
      sem_wait(&forks[0]);
      sem_wait(&forks[4]);
    }

    usleep(100 + rand() % 100); // Eating

    if (id < 4) {
      sem_signal(&forks[id + 1]);
      sem_signal(&forks[id]    );
    }
    else {
      sem_signal(&forks[4]);
      sem_signal(&forks[0]);
    }
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

