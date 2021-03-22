#include <stdio.h>     /* printf      */
#include <unistd.h>    /* fork, _exit */
#include <sys/wait.h>  /* wait        */

int main(void) {   
  int pid = fork();
  if (pid == 0) { 
    printf("Child %d\n", getpid());
    sleep(1);
    _exit(0);
  }
  printf("Parent %d waiting for %d\n",
         getpid(), pid);
  wait(NULL);
  printf("Child %d finished\n", pid);
  return 0;
}
