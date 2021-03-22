#include <stdio.h>

void swap(int * a, int * b) {
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

int main(void) {
  int x, y;
  x = 10;
  y = 20;
  printf("Before swap: x=%d, y=%d\n", x, y);
  swap(&x, &y);
  printf("After swap: x=%d, y=%d\n", x, y);
  return 0;
}
