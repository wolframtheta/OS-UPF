#include <unistd.h>   /* fork, _exit */
#include <sys/wait.h> /* wait        */

void runCommand(char *cmd) {
  char * myargv[3] = {"", "-la", NULL};
  int pid = fork();
  if (pid == 0) {
    execvp(cmd, myargv);
    _exit(1);
  }
}

// compile with "gcc Concurrent.c -o Concurrent"
// execute with "./Concurrent ls ls ls" to run several commands in parallel

int main(int argc, char *argv[]) {
  int i, pid;
  int num_cmd = argc - 1;
  for (i = 0; i < num_cmd; i++)
    runCommand(argv[i + 1]);
  while(wait(NULL) > 0);
  return 0;
}
