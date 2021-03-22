
#include "myutils.h"

#define C  5 // number of chairs
#define N 10 // number of customers

int waiting;
pthread_mutex_t mutex;    /* mutex lock         */
pthread_cond_t  barber;   /* barber condition   */
pthread_cond_t  chair;    /* chair condition    */
pthread_cond_t  customer; /* customer condition */

void * barberThread(void * params) {
  for (int k = 0; k < N; ++k) {
    pthread_mutex_lock(&mutex);

    while (waiting == 0)
      pthread_cond_wait(&customer, &mutex);

    waiting--;

    pthread_cond_signal (&barber);
    pthread_cond_signal (&chair );
    pthread_mutex_unlock(&mutex );

    usleep(100 + rand() % 100); // Cutting hair
  }
}

void * customerThread(void * params) {
  pthread_mutex_lock(&mutex);

  while (waiting == C)
    pthread_cond_wait(&chair, &mutex);

  waiting++;
  pthread_cond_signal (&customer      );
  pthread_cond_wait   (&barber, &mutex);
  pthread_mutex_unlock(&mutex         );

  usleep(100 + rand() % 100); // Getting haircut
  printf("Customer %ld got a haircut!\n", (long)params);
}

int main(int argc, char *argv[]) {
  pthread_t tids[N + 1];

  waiting = 0;
  pthread_mutex_init(&mutex   , NULL);
  pthread_cond_init (&barber  , NULL);
  pthread_cond_init (&chair   , NULL);
  pthread_cond_init (&customer, NULL);

  pthread_create(&tids[0], NULL, barberThread, NULL);
  for (long i = 1; i <= N; ++i)
    pthread_create(&tids[i], NULL, customerThread, (void *)i);

  for (long i = 0; i <= N; ++i)
    pthread_join(tids[i], NULL);

  return 0;
}

