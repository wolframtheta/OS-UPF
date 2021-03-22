#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /*open*/

#include <unistd.h> /*read, write*/


int main(int argc, char *argv[]){
	char c;
	while (read(0,&c,1)>0){
	printf("%c",c);	
	}
}
