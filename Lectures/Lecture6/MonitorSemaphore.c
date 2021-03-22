
#include "myutils.h"

// Semaphore implementation using pthread monitors
typedef struct {
  int i;
  pthread_mutex_t lock;
  pthread_cond_t cond;
} semaphore;

void sem_init(semaphore * sem, int i) {
  sem->i = i;
  pthread_mutex_init(&sem->lock, NULL);
  pthread_cond_init(&sem->cond, NULL);
}

void sem_wait(semaphore * sem) {
  pthread_mutex_lock(&sem->lock);
  while (sem->i == 0)
    pthread_cond_wait(&sem->cond, &sem->lock);
  sem->i--;
  pthread_mutex_unlock(&sem->lock);
}

void sem_signal(semaphore * sem) {
  pthread_mutex_lock(&sem->lock);
  sem->i++;
  pthread_cond_signal(&sem->cond);
  pthread_mutex_unlock(&sem->lock);
}

int sum = 0;
semaphore mutex;

void * add(void * args) {
  sem_wait(&mutex);
  sum += 1;
  sem_signal(&mutex);
}

int main(void) {
  #define NTHREADS 1000
  pthread_t tids[NTHREADS];

  sem_init(&mutex, 1);

  for (int i = 0; i < NTHREADS; ++i)
    pthread_create(&tids[i], NULL, add, NULL);

  for (int i = 0; i < NTHREADS; ++i)
    pthread_join(tids[i], NULL);

  printf("Sum is %d\n", sum);

  return 0;
}

