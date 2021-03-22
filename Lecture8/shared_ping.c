
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
sem_t * sserver;
sem_t * sclient;

int main(int argc, char * argv[]) {
  if(argc < 2) { 
    printf("usage ./a key (a shm key)\n"); 
    exit(1); 
  }

  int shmkey = atoi(argv[1]);
  printf("key %d\n", shmkey);

  shmid = shmget(shmkey, sizeof(int), 0644 | IPC_CREAT); // shared mem for writing
  int * shmpointer = shmat(shmid, NULL, 0); // attach a memory to this share memory
  if (shmid < 0 || shmpointer == NULL) {
    fprintf(stderr, "error allocating shared memory key %d\n", shmkey);
    return -1;
  }
  printf("shmpointer allocated: %p\n", shmpointer);

  int * pi = &shmpointer[0];
  *pi = 0;

  smutex  = sem_open("mutex" , O_CREAT, 0600, 1);
  sserver = sem_open("server", O_CREAT, 0600, 0);
  sclient = sem_open("client", O_CREAT, 0600, 0);

  sem_wait(sclient);
  sem_wait(smutex);
  printf("Master incremented from %d to %d\n", *pi, *pi + 1);
  *pi = *pi + 1;
  sem_post(smutex);
  sem_post(sserver);
  sleep(1);

  while (*pi < 10) {
    sem_wait(sclient);
    sem_wait(smutex);
    printf("Master incremented from %d to %d\n", *pi, *pi + 1);
    *pi = *pi + 1;
    sem_post(smutex);
    sem_post(sserver);
    sleep(1);
  }

  sem_wait(sclient);

  printf("Final result %d\n", *pi);

  shmdt(&shmpointer);

  sem_close(smutex );
  sem_close(sserver);
  sem_close(sclient);

  return 0;
}
