#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /*open*/

#include <unistd.h> /*read, write*/

int read_split(int fin, char* buff, int maxlen, char* ch_end){
	int i = 0 , oneread=1;
	char c = '.';
	while (c != ' ' && c != '\n' && oneread == 1 && i < maxlen){
		oneread = read(fin,&c,1);
		if (c != ' ' && c != '\n' && oneread == 1){
			buff[i] = c;
			i++;
		}
	}
	*ch_end = c;
	if (i < maxlen) buff[i] = '\0';
	return i;
}

int main(int argc, char *argv[]){
	char buffer[20],ch_end;
	int fin = open("words.txt",O_RDWR);
	int words=0,lines=0;
	while(read_split(fin, buffer, 100, &ch_end)> 0 ){
		printf("%s",buffer);
		if(ch_end == ' ') printf(" "); /* print space between words*/
		if(ch_end == '\n') {
			printf("\n");/* print a new line*/
			lines++;
		}
		words++;
	}
	printf("Number of words: %d and number of lines: %d\n",words,lines);
	

}
