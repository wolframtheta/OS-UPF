#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
 
int main() {
  int file = open("testfile.txt", O_RDONLY);
  if (file < -1)
    return 1;

  char buffer[20];
  if (read(file, buffer, 19) != 19)
    return 1;
  buffer[19] = '\0';
  printf("%s\n",buffer);

  if (lseek(file, 10, SEEK_SET) < 0)
    return 1;
 
  if (read(file, buffer, 19) != 19)
    return 1;
  buffer[19] = '\0';
  printf("%s\n",buffer);

  if (lseek(file, 9, SEEK_CUR) < 0)
    return 1;
 
  if (read(file, buffer, 19) != 19)
    return 1;
  buffer[19] = '\0';
  printf("%s\n",buffer);

  return 0;
}
