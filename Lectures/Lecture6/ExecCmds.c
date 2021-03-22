
#include <sys/wait.h>    /* wait */
#include "myutils.h"

int main(void) {   
  int ncmds = 2;
  char * cmds[][5] = {{"ls","-la", NULL}, {"ps","aux", NULL}};

  for (int i = 0; i < ncmds; i++) {
    int pid = fork();
    if (pid == 0) {
      execvp(cmds[i][0], cmds[i]);
      _exit(1);
    }
  }
  while (wait(NULL) > 0);

  return 0;
}
