#ifndef MYUTILS
#define MYUTILS

#include <sys/time.h>	/* gettimeofday    */
#include <unistd.h>		/* read, write     */
#include <fcntl.h>      /* open            */
#include <stdio.h>      /* printf, sprintf */
#include <string.h>     /* strlen, strcmp  */
#include <pthread.h>

void startTimer(int i);	// start timer i
long endTimer(int i);	// returns millis since timer i started

int read_split( int fin, char* buff, int maxlen, char* ch_end );
// Very similar to sys-call "read", but stops when it encounters
//   a space character ' ' or newline '\n'.
// Returns the number of characters (up to maxlen) successfully read
// Also stores in "ch_end" the last character read (' ' or '\n')

// Semaphore implementation using pthread monitors
typedef struct semaphore_struct {
	int i;
	pthread_mutex_t lock;
	pthread_cond_t cond;
} semaphore;

void sem_init(semaphore* sem, int i);
void sem_wait(semaphore* sem);
void sem_signal(semaphore* sem);

#endif
