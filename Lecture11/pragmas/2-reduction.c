#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000 /*size of a*/

void calculate(long* v) {
	for(int i=0;i<N;i++) {
		v[i] = i;
	}
}

void main(int argc, char *argv[]) {

 	int i;
 	long w, sum = 0;
 	long a[N];
 	calculate(a);
	
	/*Create the threads and starts the work-sharing construct*/
	#pragma omp parallel for private(w) reduction(+:sum) schedule(static,1)
 	for(i = 0; i < N; i++)
 	{
 		w = i*i;
 		sum = sum + w*a[i];
 	}
 	printf("sum = %li\n",sum);
 }	