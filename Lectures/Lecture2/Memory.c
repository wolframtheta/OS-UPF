#include <malloc.h>

int main(void) {
  int val;    // memory allocated on stack
  int * ptr;  // memory allocated on stack
  val = 10;   // value stored on stack
  ptr = &val; // points to value on stack

  // memory allocated on heap
  ptr = malloc(10 * sizeof(int));

  ptr[1] = 2; // value stored on heap

  // heap memory is manually deallocated
  free( ptr );

  return 0;
}
