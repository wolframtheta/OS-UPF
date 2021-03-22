#include <unistd.h>
#include <fcntl.h>

int main(void) {
  int file;

  file = open("test.txt",
              O_CREAT | O_RDWR | O_APPEND,
              0640);
  if (file < 0) return -1;
  write(1, "Written to standard output\n", 27);
  write(file, "Written to file\n", 16);
  close(file);

  return 0;
}
