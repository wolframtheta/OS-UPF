
#include "myutils.h"

Country c;

int main(int argc, char* argv[]) {
  if(argc < 4) {
    printf("Usage: ./SumCountry countries.dat 33 1000\n");
    return 1;
  }

  int fin = open(argv[1], O_RDWR, 0600);
  int i = atoi(argv[2]);
  int pop = atoi(argv[3]);

  int point = i * sizeof(Country);

  while (file_lock(fin, point, sizeof(Country)) < 0);

  lseek(fin, point, SEEK_SET);
  read(fin, &c, sizeof(Country));
  c.population += pop;
  lseek(fin, point, SEEK_SET);
  write(fin, &c, sizeof(Country));

  file_unlock(fin, point, sizeof(Country));

  close(fin);

  return 0;
}
