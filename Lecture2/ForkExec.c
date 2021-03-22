#include <stdio.h>     /* printf, sprintf */
#include <string.h>
#include <unistd.h>    /* fork, _exit     */
#include <sys/wait.h>  /* wait            */

int main(void) {
  char * myargv[3] = {"", "-la", NULL};
  int pid = fork();
  if (pid == 0) {
    execvp("/bin/ls", myargv);
    _exit(1);
  }
  wait(NULL);
  return 0;
}
