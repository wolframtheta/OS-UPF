
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int fd[2];
  pipe(fd);

  pid_t pid = fork(); /* run ls -la */
  if (pid == 0) {
    close(fd[0]);
    dup2(fd[1], 1); /* Set stdout to input of pipe */
    char * myargv[] = {"ls", "-la", NULL};
    execvp("ls", myargv);          /* and run 'ls' */
    _exit(1);
  }

  close(fd[1]);
  pid = fork();       /* run wc -l */
  if (pid == 0) {
    dup2(fd[0], 0); /* Set stdin to output of pipe */
    char * myargv[] = {"wc", "-l", NULL};
    execvp("wc", myargv);
    _exit(1);
  }
    
  while (wait(NULL) > 0); /* Wait for wc to finish */

  return 0;
}
