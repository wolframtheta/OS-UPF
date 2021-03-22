
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  int num;
  int fd[2][2];

  for (int i = 0; i < 2; i++)
    pipe(fd[i]);    

  for (int i = 0; i < 2; i++) {
    int pid = fork();
    if (pid == 0) {
      close(fd[i][1]);
      read(fd[i][0], &num, sizeof(int));
      printf("Child %d read %d\n", getpid(), num);
      if (i == 0) {
        num++;
        printf("Child %d wrote %d\n", getpid(), num);
        write(fd[1][1], &num, sizeof(int));
      }
      _exit(0);
    }
  }

  close(fd[0][0]);
  num = 1000;
  printf("Parent wrote %d\n", num);
  write(fd[0][1], &num, sizeof(int));

  while (wait(NULL) > 0);

  return 0;
}

