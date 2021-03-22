
#ifndef MYUTILS
#define MYUTILS

#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>     // unix-like system calls read and write
#include <fcntl.h>      // unix-like file handling : open
#include <stdlib.h>     // standard C lib input output basic functions compatible with Windows
#include <stdio.h>      // standard C lib input output basic functions compatible with Windows
#include <string.h>     // also from standard C lib : basic string functions like strlen
#include <pthread.h>

void startTimer(int i);   // start timer i
long endTimer(int i);     // returns millis since timer i started

int read_split( int fin, char* buff, int maxlen, char* ch_end );

#endif
