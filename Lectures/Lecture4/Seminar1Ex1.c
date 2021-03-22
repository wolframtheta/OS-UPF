
#include <stdlib.h>
#include "myutils.h"

int main(int argc, char * argv[]) {
	if (argc < 2) {
		write(1, "Missing filename\n", 17);
		exit(1);
	}

	int file = open(argv[1], O_RDONLY);
	if (file < 0) {
		write(1, "File does not exist\n", 20);
		exit(1);
	}

	char buff[100], ch_end;
	int nwords = 0, nlines = 0;
	while (read_split(file, buff, 100, &ch_end) > 0) {
		if (strlen(buff) > 0) {
			nwords++;
			write(1, buff, strlen(buff));
		}
		if (ch_end == '\n') nlines++;
		write(1, &ch_end, 1);
	}
	close(file);

	sprintf(buff, "Number of words: %d\n", nwords);
	write(1, buff, strlen(buff));
	sprintf(buff, "Number of lines: %d\n", nlines);
	write(1, buff, strlen(buff));

	return 0;
}
