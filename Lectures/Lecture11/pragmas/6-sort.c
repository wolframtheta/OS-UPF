#include <stdio.h>      /* printf */
#include <stdlib.h>     /* atoi */
#include <stdbool.h>    /* bool type */
#include <omp.h>
#include <time.h>       // for usleep : waits for microseconds
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


struct timeval start;
void startTimer();
long endTimer();


int main(int argc, char *argv[]) {
    long N = 100000;
    bool bParallel = true;
    if(argc > 1) bParallel = (bool) atoi(argv[1]);
    if(argc > 2) N = atol(argv[2]);

    long a[N];
    
    long i;
    for(i = 0; i < N; i++) {
        a[i] = rand() % N;
    }

    long j, amin, jmin;

    startTimer();

    if(!bParallel) {
        for (i = 0; i < N; i++) {
            amin = N;
            for( j = i; j < N; j++) {
                if(a[j] < amin) { amin = a[j]; jmin = j; }
            }
            long tmp = a[i];
            a[i] = a[jmin];
            a[jmin] = tmp;
        } 
        if(N<100) {
          printf("a = ");
          for (i = 0; i < N; i++) printf("%ld ",a[i]);
          printf("\n");
        }
        printf("finished normal for; %ld elements\n",N);
        printf("Done in %ld millis\n",endTimer());
        return 0;
    }

    //omp_set_num_threads(4); 
  
    for(i=0;i<N;i++) {
      int first = i % 2;
      #pragma omp parallel for default(none), shared(a,first,N)
      for (j=first; j<N-1; j+=2) {
        if (a[j] > a[j+1]) {
          long tmp = a[j];
          a[j] = a[j+1];
          a[j+1] = tmp;
        }
      }
    }
    
    if(N<100) {
       printf("a = ");
       for (i = 0; i < N; i++) printf("%ld ",a[i]);
       printf("\n");
    }
    printf("finished with parallel for; %ld elements\n",N);
    printf("Done in %ld millis\n",endTimer());
    return 0;
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
