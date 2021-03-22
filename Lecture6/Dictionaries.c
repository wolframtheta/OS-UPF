
#include "myutils.h"

#define MAX_DICTS 5

int files[MAX_DICTS];
int nwords[MAX_DICTS];
char * words[MAX_DICTS][1000]; 
pthread_t tids[MAX_DICTS];

void * load_dict(void * param) {
  int * params = (int *)param;
  int d = params[0];
  int file = params[1];

  int n = 0;
  int newline = 1;
  char buff[100], ch_end;
  while (read_split(file, buff, 100, &ch_end) > 0) {
    if (newline) {
      words[d][n] = malloc(100);
      memcpy(words[d][n], buff, strlen(buff) + 1);
      n++;
    }
    newline = ch_end == '\n';
  } 
  nwords[d] = n;
  free(params);
  return NULL;
}

void print_dict(int d) {
  printf("--------- Printing Dict %d\n", d);
  for (int i = 0; i < nwords[d]; i++) {
    printf("word %d is %s\n", i, words[d][i]);
    free(words[d][i]);
  }
  printf("--------- End of Dict %d\n", d);
}

int main(int argc, char * argv[]) {
  if (argc <= 1) {
    printf("Usage: dicts to load as arguments, example:\n");
    printf("Usage: ./main Dict1.txt Dict2.txt ...\n");
    exit(1);
  }

  if (MAX_DICTS + 1 < argc) {
    printf("Number of dictionaries exceeds limit (%d)\n", MAX_DICTS);
    exit(1);
  }

  for (int i = 0; i < argc - 1; i++) {
    nwords[i] = 0;
    files[i] = open(argv[i + 1], O_RDONLY);
    if (files[i] < 0) {
      printf("File %s does not exist\n", argv[i + 1]);
      exit(1);
    }

    int * params = malloc(2 * sizeof(int));
    params[0] = i;
    params[1] = files[i];
    pthread_create(&tids[i], NULL, load_dict, params);
  }

  for (int i = 0; i < argc - 1; i++) {
    pthread_join(tids[i], NULL);
    print_dict(i);
    close(files[i]);
  }

  return 0;
}

