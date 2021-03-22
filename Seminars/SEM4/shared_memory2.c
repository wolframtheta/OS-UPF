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

int main(int argc, char * argv[]) {
  
  int shm_id = shmget(1234,sizeof(int),IPC_CREAT | 0666);
  int *shm = shmat(shm_id,NULL,0);
  //lock the process using sem
  sem_t *named_mutex = sem_open("named_mutex",O_CREAT,0600,1);
  
   
  for (int i = 0; i<1000; i++){
    sem_wait();
    *shm += 1;
    sem_post(named_mutex)
  }
  printf("Sum: %d \n",*sum);
  sem_close(closed_mutex);
  shmctl(shm_id,IPC_RMID,NULL);//removed the shared memory IMPORTANT !!
  return 0;
}
