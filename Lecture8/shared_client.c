
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <math.h>

int shmid;

sem_t * smutex;
sem_t * sgo;
sem_t * swait;

int main(int argc, char * argv[]) {
  if(argc < 3) { 
    printf("usage ./a key num (a shm key and the number of increments)\n"); 
    exit(1); 
  }

  int shmkey = atoi(argv[1]);
  int n = atoi(argv[2]);
  printf("key %d num %d\n", shmkey, n);

  shmid = shmget(shmkey, sizeof(int), 0600);  // shared mem for reading
  int * shmpointer = shmat(shmid, NULL, 0); // attach a memory to this shared memory
  if (shmid < 0 || shmpointer == NULL) {
    fprintf(stderr, "error allocating shared memory key %d\n", shmkey);
    return -1;
  }
  printf("shmpointer allocated: %p\n", shmpointer);

  int * pi = &shmpointer[0];

  smutex = sem_open("mutex", O_RDWR, 0600, 1);
  sgo    = sem_open("go"   , O_RDWR, 0600, 0);
  swait  = sem_open("wait" , O_RDWR, 0600, 0);

  printf("signal on sem go\n");
  sem_post(sgo);

  printf("wait on sem mutex\n");
  printf("increment\n");
  for (int i = 0; i < n; i++) {
    sem_wait(smutex);
    *pi = *pi + 1;
    sem_post(smutex);
    usleep(10);
  }

  printf("I finished; temporal result %d\n", *pi);

  sem_post(swait);

  shmdt(&shmpointer);

  sem_close(smutex);
  sem_close(sgo   );
  sem_close(swait );

  return 0;
}
