#ifndef MYUTILS
#define MYUTILS

#include <sys/time.h>	/* gettimeofday    */
#include <unistd.h>		/* read, write     */
#include <fcntl.h>      /* open            */
#include <stdio.h>      /* printf, sprintf */
#include <string.h>     /* strlen, strcmp  */

void startTimer(int i);	// start timer i
long endTimer(int i);	// returns millis since timer i started

int read_split( int fin, char* buff, int maxlen, char* ch_end );
// Very similar to sys-call "read", but stops when it encounters
//   a space character ' ' or newline '\n'.
// Returns the number of characters (up to maxlen) successfully read
// Also stores in "ch_end" the last character read (' ' or '\n')

#endif
