
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int num;
  int fd[2];
  pipe(fd);

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[1]);
      read(fd[0], &num, sizeof(int));
      printf("Child %d read %d\n", getpid(), num);
      _exit(1);
    }
  }

  close(fd[0]);
  for (int i = 0; i < 3; i++) {
    num = 1000 + i;
    printf("Parent wrote %d\n", num);
    write(fd[1], &num, sizeof(int));
    sleep(1);
  }

  while (wait(NULL) > 0);

  return 0;
}

