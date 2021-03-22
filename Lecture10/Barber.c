
#include "myutils.h"

#define C  5 // number of chairs
#define N 10 // number of customers

int waiting;
semaphore barber;
semaphore customer;
semaphore mutex;

void * barberThread(void * params) {
  for (int k = 0; k < N; ++k) {
    sem_wait(&customer);
    sem_wait(&mutex   );

    waiting--;

    sem_signal(&barber);
    sem_signal(&mutex );

    usleep(100 + rand() % 100); // Cutting hair
  }
}

void * customerThread(void * params) {
  int cuthair = 0;
  while (!cuthair) {
    sem_wait(&mutex);
    if (waiting < C) {
      waiting++;
      sem_signal(&customer);
      sem_signal(&mutex   );
      sem_wait  (&barber  );

      cuthair = 1;
      usleep(100 + rand() % 100); // Getting haircut
      printf("Customer %ld got a haircut!\n", (long)params);
	}
    else sem_signal(&mutex);
  }
}

int main(int argc, char *argv[]) {
  pthread_t tids[N + 1];

  waiting = 0;
  sem_init(&barber  , 0);
  sem_init(&customer, 0);
  sem_init(&mutex   , 1);

  pthread_create(&tids[0], NULL, barberThread, NULL);
  for (long i = 1; i <= N; ++i)
    pthread_create(&tids[i], NULL, customerThread, (void *)i);

  for (long i = 0; i <= N; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

