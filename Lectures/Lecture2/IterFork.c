#include <stdio.h>     /* printf, sprintf */
#include <string.h>    /* strlen          */
#include <unistd.h>    /* fork, _exit     */
#include <sys/wait.h>  /* wait            */

void doTask() {
  char buffer[128];
  sprintf(buffer, "Child %d, parent %d\n",
          getpid(), getppid());
  write(1, buffer, strlen(buffer));
  _exit(0);
}

int main(void) {
  int i, pid;
  char buffer[128];
  sprintf(buffer, "My PID is %d\n", getpid());
  write(1, buffer, strlen(buffer));
  for (i = 0; i < 3; i++) {
    pid = fork();
    if(pid == 0) doTask();
  }
  for (i = 0; i < 3; i++) {
    int pid_child = wait(NULL);
    printf("Finished child %d\n", pid_child);
  }
  return 0;
}
