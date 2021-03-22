
#include <stdlib.h>    /* rand            */
#include "myutils.h"   /* timers          */

semaphore mutex;       /* mutex semaphore */
semaphore emptySlots;  /* empty semaphore */
semaphore filledSlots; /* full semaphore  */

#define N 10
int buffer[N];         /* circular buffer */
int in = 0;
int out = 0;
int count = 0;

void * producer(void * param) {
  int n = atoi(param); // number of items to be produced
  for (int i = 0; i < n; ) {
    usleep(5000 + rand() % 100); // ProduceItem()

    sem_wait(&emptySlots);
    sem_wait(&mutex);

    buffer[in] = ++i; // add item
    printf("Thread %ld: ", pthread_self());
    printf("Produced %d and inserted at %d\n", i, in);
    in = (in + 1) % N;

    sem_signal(&mutex);
    sem_signal(&filledSlots);
  }
}

void * consumer(void * param) {
  int n = atoi(param); // number of items to be consumed
  for (int i = 0; i < n; ) {
    sem_wait(&filledSlots);
    sem_wait(&mutex);       
                
    i = buffer[out];  // take item
    printf("Thread %ld: ", pthread_self());
    printf("Took %d from position %d\n", i, out);
    out = (out + 1) % N;

    sem_signal(&mutex);
    sem_signal(&emptySlots);
 
    usleep(5000 + rand() % 100); // ConsumeItem()
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: ./pc <integer value>\n");
    return -1;
  }

  #define NThreads 10
  pthread_t tid[NThreads];

  sem_init(&emptySlots, N);
  sem_init(&filledSlots, 0);
  sem_init(&mutex, 1);

  startTimer(0);
  for (int i = 0; i < NThreads; i++) {
    if (i % 2 == 0) pthread_create(&tid[i], NULL, producer, argv[1]);
    if (i % 2 == 1) pthread_create(&tid[i], NULL, consumer, argv[1]);
  }

  for (int i = 0; i < NThreads; i++)
    pthread_join(tid[i], NULL);

  printf("Time: %ld\n", endTimer(0));
  return 0;
}
