
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double** createMatrix( int n, int m) {
    int i;
    double** mat = malloc( n * sizeof(double*) );
    for(i=0;i<n;i++) {
        mat[i] = malloc( m * sizeof(double) );
    }
    return mat;
}


int NRA = 10;                 /* number of rows in matrix A */
int NCA = 11;                 /* number of columns in matrix A */
int NCB = 12;                 /* number of columns in matrix B */

double** a;
double** b;
double** c;

int main (int argc, char *argv[]) 
{
  int tid, nthreads, i, j, k, chunk;

  a = createMatrix(NRA,NCA);
  b = createMatrix(NCA,NCB);
  c = createMatrix(NRA,NCB);

  chunk = 100;                    /* set loop iteration chunk size */

  /*** Spawn a parallel region explicitly scoping all variables ***/
  #pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0) {
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple example with %d threads\n",nthreads);
    printf("Initializing matrices...\n");
  }
   
  #pragma omp for schedule (static, chunk)   /*** Initialize matrices ***/
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /*** Do matrix multiply sharing iterations on outer loop ***/
  /*** Display who does which iterations for demonstration purposes ***/
  printf("Thread %d starting matrix multiply...\n",tid);
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)    
    {
    printf("Thread=%d did row=%d\n",tid,i);
    for(j=0; j<NCB; j++)       
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   /*** End of parallel region ***/



/*** Print results ***/
printf("******************************************************\n");
printf("Result Matrix %dx%d:\n", NRA, NCB);
/*for (i=0; i<NRA; i++) {
  for (j=0; j<NCB; j++) 
    printf("%6.2f   ", c[i][j]);
    printf("\n"); 
}*/
printf("******************************************************\n");
printf ("Done\n");

}





