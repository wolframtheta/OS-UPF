
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

	int nwords = 0;
	char words[1000][100], buff[100], ch_end;
	while (read_split(file, buff, 100, &ch_end) > 0)
		if (strlen(buff) > 0 &&
        strcspn(buff, "0123456789") == strlen(buff)) {
			memcpy(words[nwords], buff, strlen(buff));
			nwords++;
		}
	close(file);

	for (int i = 0; i < nwords; ++i) {
		if (i) write(1, " ", 1);
		write(1, words[i], strlen(words[i]));
	}
	write(1, "\n", 1);

	return 0;
}
