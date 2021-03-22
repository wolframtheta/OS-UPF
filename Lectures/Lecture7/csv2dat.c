
#include "myutils.h"

#define MAXL 200

Country c;

int main(void) {
  int fin  = open("countries.csv", O_RDONLY);
  int fout = open("countries.dat", O_CREAT | O_RDWR, 0600);
  if (fin < 0) return -1;

  int icol = 0;
  char buff[MAXL], ch_end;
  while (read_split(fin, buff, MAXL, &ch_end) > 0) {
    if (ch_end == ';') {
      if(icol == 0) sprintf(c.name, "%s", buff);
      if(icol == 6) c.population = atoi(buff);
      if(icol == 7) c.area = atoi(buff);
      if(icol == 8) c.coastline = atoi(buff);
      icol++;
    }

    if(ch_end == '\n') {
      icol = 0;
      write(fout, &c, sizeof(c));
    }
  }

  close( fin );
  close( fout );

  return 0;
}
