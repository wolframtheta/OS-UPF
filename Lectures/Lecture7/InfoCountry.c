
#include "myutils.h"

Country c;

int main(int argc, char* argv[]) {
  printf("Size of Country struct: %lu\n", sizeof(Country));

  if (argc < 3) {
    printf("Usage: ./InfoCountry countries.dat 33\n");
    return 1;
  }

  int i = atoi(argv[2]);
  int fin = open(argv[1], O_RDONLY);

  int point = i * sizeof(Country);
  lseek(fin, point, SEEK_SET);
  read(fin, &c, sizeof(Country));
  printCountry(&c);

  close(fin);

  return 0;
}
