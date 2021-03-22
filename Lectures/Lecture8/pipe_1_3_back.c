
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int num;
  int fd[2];
  int fd_back[2];
  pipe(fd);
  pipe(fd_back);

  for (int i = 0; i < 3; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[1]);
      close(fd_back[0]);
      read(fd[0], &num, sizeof(int));
      printf("Child %d read %d\n", getpid(), num);
      num++;
      sleep(1);
      write(fd_back[1], &num, sizeof(int));
      _exit(1);
    }
  }

  close(fd[0]);
  close(fd_back[1]);
  for (int i = 0; i < 3; i++) {
    num = 1000 + i;
    printf("Parent wrote %d\n", num);
    write(fd[1], &num, sizeof(int));
    sleep(1);
  }

  for (int i = 0; i < 3; i++) {
    read(fd_back[0], &num, sizeof(int));
    printf("Parent read %d\n", num);
  }

  while (wait(NULL) > 0);

  return 0;
}

