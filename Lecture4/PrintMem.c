
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>

typedef enum {
	INT   = 0,
	CHAR  = 1,
	FLOAT = 2
} Type;

typedef union {
	int   * iptr;
	char  * cptr;
	float * fptr;
	void  * vptr;
} Pointer;

void printMem(void * pointer, Type type, int num) {
	Pointer ptr = (Pointer)pointer;
	for (int i = 0; i < num; i++) {
		switch (type) {
		case INT  : printf("%d:\t%p\t%d\n", i, ptr.iptr + i, ptr.iptr[i]); break;
		case CHAR : printf("%d:\t%p\t%c\n", i, ptr.cptr + i, ptr.cptr[i]); break;
		case FLOAT: printf("%d:\t%p\t%f\n", i, ptr.fptr + i, ptr.fptr[i]); break;
		}
	}
}

int main(void) {
	int   * intPtr   = malloc(10 * sizeof(int));
	char  * charPtr  = malloc(10);
	float * floatPtr = malloc(10 * sizeof(float));

	for (int i = 0; i < 10; ++i) {
		intPtr  [i] = i;
		charPtr [i] = (char)('A' + i);
		floatPtr[i] = i / 10.0;
	}

	printMem(intPtr  , INT  , 10);
	write(1, "\n", 1);
	printMem(charPtr , CHAR , 10);
	write(1, "\n", 1);
	printMem(floatPtr, FLOAT, 10);

	free(intPtr  );
	free(charPtr );
	free(floatPtr);

	return 0;
}
