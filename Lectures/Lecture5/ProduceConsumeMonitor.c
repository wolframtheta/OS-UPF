
#include <stdlib.h>          /* rand            */
#include "myutils.h"         /* timers          */

pthread_mutex_t mutex;       /* mutex lock      */
pthread_cond_t  emptySlots;  /* empty condition */
pthread_cond_t  filledSlots; /* full condition  */

#define N 10
int buffer[N];               /* circular buffer */
int in = 0;
int out = 0;
int count = 0;

void * producer(void * param) {
  int n = atoi(param); // number of items to be produced
  for (int i = 0; i < n; ) {
    usleep(5000 + rand() % 100); // ProduceItem()

    pthread_mutex_lock(&mutex);

    while (count == N)
      pthread_cond_wait(&emptySlots, &mutex);        

    count++;

    buffer[in] = ++i; // add item
    printf("Thread %ld: ", pthread_self());
    printf("Produced %d and inserted at %d\n", i, in);
    in = (in + 1) % N;

    pthread_cond_signal(&filledSlots);
    pthread_mutex_unlock(&mutex);
  }
}

void * consumer(void * param) {
  int n = atoi(param); // number of items to be consumed
  for (int i = 0; i < n; ) {
    pthread_mutex_lock(&mutex);

    while (count == 0)
      pthread_cond_wait(&filledSlots, &mutex);

    count--;      
                
    i = buffer[out];  // take item
    printf("Thread %ld: ", pthread_self());
    printf("Took %d from position %d\n", i, out);
    out = (out + 1) % N;

    pthread_cond_signal(&emptySlots);
    pthread_mutex_unlock(&mutex);
 
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

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&emptySlots, NULL);
  pthread_cond_init(&filledSlots, NULL);

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
