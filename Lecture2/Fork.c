#include <stdio.h>   /* sprintf */
#include <string.h>  /* strlen  */
#include <unistd.h>  /* fork    */

int main(void) {
  int id;
  char buffer[128];
  id = fork();
  sprintf(buffer, "fork returns %d\n", id);
  write(1, buffer, strlen(buffer));
  return 0;
}
