
#include <sys/wait.h>    /* wait */
#include "myutils.h"

#define ROWS 5
#define COLS 10

char names[ROWS][COLS]= { "tree",
                          "bowl",
                          "hat",
                          "mice",
                          "toon" };

char ** words;

void print_2d_mem(char ** a, int n) {
  printf("Printing %d strings\n", n);
  for (int i = 0; i < n; i++)
    printf("> %d string: %s\n", i, a[i]);
}

char ** copy_2d_array(char input[ROWS][COLS], int n) {
  char ** out = malloc(n * sizeof(char *));
  for (int i = 0; i < n; i++) {
    out[i] = malloc(strlen(input[i]) + 1);
    memcpy(out[i], input[i], strlen(input[i]) + 1);  
  }
  return out;
}

char ** link_2d_array(char input[ROWS][COLS], int n) {
  char ** out = malloc(n * sizeof(char *));
  for (int i = 0; i < n; i++)
  	out[i] = input[i];
  return out;
}

void free_2d_mem(char ** mem, int n) {
  int i;
  for (int i = 0; i < n; i++)
    free(mem[i]);
  free(mem);
}

int main(void) {
  printf("Print names 2D Array Size in bytes: %lu\n", sizeof(names));
  for (int i = 0; i < ROWS; i++)
    printf("> %d name address: %p\n", i, names[i]);

  words = copy_2d_array(names, 5);

  names[0][0] = 'T';

  print_2d_mem(words, 5);
  free_2d_mem(words, 5);

  words = link_2d_array(names,5);

  print_2d_mem(words, 5);
  names[1][0] = 'B';
  print_2d_mem(words, 5);

  free(words);

  return 0;
}

