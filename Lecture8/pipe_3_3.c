
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int num;
  int fd[3][2];

  for (int i = 0; i < 3; i++) {
    pipe(fd[i]);
    int pid = fork();
    if (pid == 0) {
      close(fd[i][1]);
      read(fd[i][0], &num, sizeof(int));
      printf("Child %d read %d\n", getpid(), num);
      _exit(1);
    }
  }

  for (int i = 0; i < 3; i++) {
    close(fd[i][0]);
    num = 1000 + i;
    printf("Parent wrote %d\n", num);
    write(fd[i][1], &num, sizeof(int));
    sleep(1);
  }

  while (wait(NULL) > 0);

  return 0;
}

