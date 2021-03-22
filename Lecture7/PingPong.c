
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(void) {
  int fd[2];
  int fd_back[2];

  pipe(fd);      // pipe creation before the fork
  pipe(fd_back);
    
  int num = 0;
  int ret = fork();

  if (ret == 0) {
    // Child code
    close(fd[1]);
    close(fd_back[0]);

    // Child reads in the pipe fd[0] and writes in fd_back[1]
    while (read(fd[0], &num, sizeof(int)) > 0) {
      printf("Child Received: %d\n", num);
      num = num < 10 ? num + 1 : -1;
      printf("Child Sent: %d\n", num);
      write(fd_back[1], &num, sizeof(int));
    }
 
    exit(0);
  }

  // Father code (since child code exits)
  close(fd[0]);
  close(fd_back[1]);

  // Father reads in the pipe fd_back[0] and writes in fd[1]
  printf("Father Sent: %d\n", num);
  write(fd[1], &num, sizeof(int));
  while (num >= 0 && read(fd_back[0], &num, sizeof(int)) > 0) {
    printf("Father Received: %d\n", num);
    if (num >= 0) {
      num = num + 1;
      printf("Father Sent: %d\n", num);
      write(fd[1], &num, sizeof(int));
    }
  }

  return 0;
}
