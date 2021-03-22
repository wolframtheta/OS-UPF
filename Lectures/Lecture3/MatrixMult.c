#include <assert.h>  /* assert         */
#include <malloc.h>  /* malloc, free   */
#include <pthread.h> /* pthread_create */
#include <stdlib.h>  /* atoi           */
#include "myutils.h"

// struct for representing a matrix
typedef struct {
  int rows, cols;
  int ** vals;
} Matrix;

// function for creating a matrix and filling with 0s 
Matrix * createMatrix(int r, int c) {
  // allocate memory of the struct
  Matrix * m = malloc(sizeof(Matrix));

  // assign rows and columns
  m->rows = r;
  m->cols = c;

  // allocate memory for r rows
  m->vals = malloc(r * sizeof(int *));
  for (int i = 0; i < r; ++i) {
    // for each row, allocate memory for c integers
    m->vals[i] = malloc(c * sizeof(int));

    // initialize all elements in a row to 0
    // only works for initializing to 0!
    memset(m->vals[i], 0, c * sizeof(int));
  }

  return m;
}

// function for deallocating the memory of a matrix
void freeMatrix(Matrix * m) {
  for (int i = 0; i < m->rows; ++i)
    // free the memory allocated for each row
    free(m->vals[i]);

  // free the memory of the rows
  free(m->vals);

  // free the memory of the struct
  free(m);
}

// function for reading a matrix from a file "filename"
Matrix * readMatrix(char * filename) {
  char ch_end;
  char buffer[100];

  // open the file
  int file = open(filename, O_RDONLY);

  // read the first word and convert to integer (rows)
  read_split(file, buffer, 100, &ch_end);
  int r = atoi(buffer);

  // read the next word and convert to integer (cols)
  read_split(file, buffer, 100, &ch_end);
  int c = atoi(buffer);

  // create the matrix filled with 0s
  Matrix * m = createMatrix(r, c);

  for (int i = 0; i < r; ++i)
    for (int j = 0; j < c; ++j) {
      // for each row and column, read element and store in matrix
      read_split(file, buffer, 100, &ch_end);
      m->vals[i][j] = atoi(buffer);
    }

  // close the file
  close(file);

  return m;
}

// function for writing a matrix to the screen
void writeMatrix(Matrix * m) {
  char buffer[100];

  // write the number of rows and columns on a line
  sprintf(buffer, "%d %d\n", m->rows, m->cols);
  write(1, buffer, strlen(buffer));

  for (int i = 0; i < m->rows; ++i) {
    // for each row, create a string containing all elements
    sprintf(buffer, "%d", m->vals[i][0]);
    for (int j = 1; j < m->cols; ++j)
      sprintf(buffer + strlen(buffer), " %d", m->vals[i][j]);
    sprintf(buffer + strlen(buffer), "\n");

    // write the resulting string on a line
    write(1, buffer, strlen(buffer));
  }
}

// struct containing arguments to the function "multiplyRow"
typedef struct {
  int row;
  Matrix * A, * B, * C;
} MatrixMult;

// function for computing one row of a matrix multiplication
void * multiplyRow(void * arg) {
  // convert argument to a pointer to MatrixMult
  MatrixMult * mm = (MatrixMult *)arg;

  // iterate over the elements of the row
  for (int j = 0; j < mm->B->cols; ++j)
    // fill each element with the correct value
    // assumes initialization to 0!
    for (int k = 0; k < mm->A->cols; ++k)
      mm->C->vals[mm->row][j] += mm->A->vals[mm->row][k] * mm->B->vals[k][j];
}

// function for multiplying two matrices A and B
Matrix * multiply(Matrix * A, Matrix * B) {
  // check that the dimensions are correct
  assert(A->cols == B->rows);

  // create the result matrix and fill with 0s
  Matrix * C = createMatrix(A->rows, B->cols);
  
  // allocate memory for the arguments and thread IDs
  MatrixMult * mms = malloc(A->rows * sizeof(MatrixMult));
  pthread_t * tids = malloc(A->rows * sizeof(pthread_t));

  for (int i = 0; i < A->rows; ++i) {
    // for each row, configure the arguments
    mms[i].row = i;
    mms[i].A = A;
    mms[i].B = B;
    mms[i].C = C;

    // then create a thread that computes this row
    pthread_create(&tids[i], NULL, multiplyRow, &mms[i]);
  }

  // wait for all threads to finish
  for (int i = 0; i < A->rows; ++i)
    pthread_join(tids[i], NULL);

  // free the memory for arguments and thread IDs
  free(mms);
  free(tids);

  return C;
}

int main(void) {
  // read matrices A and B from files
  Matrix * A = readMatrix("m1.txt");
  Matrix * B = readMatrix("m2.txt");

  // compute the result of multiplying A and B
  Matrix * C = multiply(A, B);

  // write the matrices to the screen, separated by newline
  writeMatrix(A);
  write(1, "\n", 1);
  writeMatrix(B);
  write(1, "\n", 1);
  writeMatrix(C);

  // free the memory of all matrices
  freeMatrix(A);
  freeMatrix(B);
  freeMatrix(C);
}

