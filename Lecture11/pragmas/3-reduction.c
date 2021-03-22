/******************************************************************************
* FILE: omp_reduction.c
* DESCRIPTION:
*   OpenMP Example - Combined Parallel Loop Reduction - C/C++ Version
*   This example demonstrates a sum reduction within a combined parallel loop
*   construct.  Notice that default data element scoping is assumed - there
*   are no clauses specifying shared or private variables.  OpenMP will 
*   automatically make loop index variables private within team threads, and
*   global variables shared.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/

#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <time.h>       /* clock */
#include <stdbool.h>    /* bool type */
#include <math.h>      /* cos, sin */
#include <time.h>    // for usleep : waits for microseconds
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


struct timeval start;
void startTimer();
long endTimer();


int main (int argc, char *argv[]) 
{
    long   i,j,k;
    long N = 100000;
    bool bParallel = true;
    if(argc > 1) bParallel = (bool) atoi(argv[1]);
    if(argc > 2) N = atol(argv[2]);
    
    float   a[N], b[N];
    double  sum;


    startTimer();

    /* Some initializations */
    for (i=0; i < N; i++) a[i] = b[i] = i * 1.0;
    sum = 0.0;

    if(bParallel) {
        #pragma omp parallel for reduction(+:sum)
        for (i=0; i < N; i++) {
            sum = sum + (a[i] * b[i]);
            usleep(100);
        }
    }
    else {
        for (i=0; i < N; i++) {
            sum = sum + (a[i] * b[i]);
            usleep(100);
        }
    }
       
    printf("\nSum = %lf\n",sum);
    if(bParallel) printf("with parallel for; %ld elements\n",N);
    else          printf("normal for; %ld elements\n",N);
    printf("Done in %ld millis\n",endTimer());
}





void startTimer() {
  gettimeofday(&start, NULL);
}

long endTimer() {
  long mtime, seconds, useconds;    
  struct timeval end;
  gettimeofday(&end, NULL);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
  return mtime;
}





