#include "myutils.h"

struct timeval start[100];

void startTimer(int i) {
  if(i>=100) printf("Warning only 100 timers available!!");
  gettimeofday(&start[i], NULL);
}

long endTimer(int i) {
  long mtime, seconds, useconds;    
  struct timeval end;

  gettimeofday(&end, NULL);
  seconds  = end.tv_sec  - start[i].tv_sec;
  useconds = end.tv_usec - start[i].tv_usec;
  mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;

  return mtime;
}

int read_split(int fin, char * buff, int maxlen, char * ch_end) {
  int i;
  char * whitespace = 0;
  for (i = 0; i < maxlen && !whitespace && read(fin, ch_end, 1); i++) {
    whitespace = strchr(" \n\t&", *ch_end);
    if (!whitespace) buff[i] = *ch_end;
  }
  if (whitespace) buff[i - 1] = '\0';
  return i;
}

void sem_init(semaphore* sem, int i) {
  sem->i = i;
  pthread_mutex_init(&sem->lock, NULL);
  pthread_cond_init(&sem->cond, NULL);
}

void sem_wait(semaphore* sem) {
   pthread_mutex_lock(&sem->lock);
   while(sem->i == 0) {
    pthread_cond_wait(&sem->cond, &sem->lock);
   }
   sem->i--;
   pthread_mutex_unlock(&sem->lock);
}

void sem_signal(semaphore* sem) {
   pthread_mutex_lock(&sem->lock);
   sem->i++;
   pthread_cond_signal(&sem->cond);
   pthread_mutex_unlock(&sem->lock);
}

