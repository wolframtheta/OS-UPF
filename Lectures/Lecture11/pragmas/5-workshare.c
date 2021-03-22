/******************************************************************************
* FILE: omp_workshare1.c
  from: https://computing.llnl.gov/tutorials/openMP/samples/C/omp_workshare1.c
 
* DESCRIPTION:
*   OpenMP Example - Loop Work-sharing - C/C++ Version
*   In this example, the iterations of a loop are scheduled dynamically
*   across the team of threads.  A thread will perform CHUNK iterations
*   at a time before being scheduled for the next CHUNK of work.
* AUTHOR: Blaise Barney  5/99
* LAST REVISED: 04/06/05
******************************************************************************/
#include <omp.h>
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <time.h>       /* clock */
#include <stdbool.h>    /* bool type */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


struct timeval start;
void startTimer();
long endTimer();


#define CHUNKSIZE   10

int main (int argc, char *argv[]) 
{
    long N = 100;
    int nthreads, tid, i, chunk;
    bool bParallel = true;
    
    if(argc > 1) bParallel = (bool) atoi(argv[1]);
    if(argc > 2) N = atol(argv[2]);
    
    float a[N], b[N], c[N];
    
    startTimer();

    /* Some initializations */
    for (i=0; i < N; i++) a[i] = b[i] = i * 1.0;
    chunk = CHUNKSIZE;

    if(bParallel) {
        #pragma omp parallel shared(a,b,c,nthreads,chunk) private(i,tid)
        {
            tid = omp_get_thread_num();
            if (tid == 0) {
                nthreads = omp_get_num_threads();
            }
            #pragma omp for schedule(dynamic,chunk)
            for (i=0; i<N; i++) {
                c[i] = a[i] + b[i];
                usleep(10);
                printf("Thread %d: c[%d]= %f\n",tid,i,c[i]);
            }
        }  /* end of parallel section */
    } else {
        for (i=0; i<N; i++) {
            c[i] = a[i] + b[i];
            usleep(10);
            printf("c[%d]= %f\n",i,c[i]);
        }
    }
    
    if(bParallel) printf("with parallel for; %ld\n",N);
    else          printf("normal for; %ld\n",N);
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


