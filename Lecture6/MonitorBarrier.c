
#include "myutils.h"

#define NTHREADS 10

typedef struct {
  int n;
  pthread_mutex_t lock;
  pthread_cond_t cond;
} barrier;

void barrier_init(barrier * b, int i) {
  b->n = i;
  pthread_mutex_init(&b->lock, NULL);
  pthread_cond_init(&b->cond, NULL);
}

void wait_for_others(barrier * b) {
  pthread_mutex_lock(&b->lock);
  b->n--;
  
  while (b->n > 0)
    pthread_cond_wait(&b->cond, &b->lock);

  pthread_cond_broadcast(&b->cond);
  pthread_mutex_unlock(&b->lock);
}

int sum = 0;
pthread_mutex_t lock;
barrier mybarrier;

void * addtwice(void * args) {
  pthread_mutex_lock(&lock);
  sum += 1;
  pthread_mutex_unlock(&lock);

  wait_for_others(&mybarrier);

  pthread_mutex_lock(&lock);
  sum += 1;
  pthread_mutex_unlock(&lock);
}

int main(void) {
  pthread_t tids[NTHREADS];
  pthread_mutex_init(&lock, NULL);
  barrier_init(&mybarrier, NTHREADS);

  for (int i = 0; i < NTHREADS; i++)
    pthread_create(&tids[i], NULL, addtwice, NULL);

  for (int i = 0; i < NTHREADS; i++)
    pthread_join(tids[i], NULL);

  printf("Sum is %d\n", sum);

  return 0;
}

