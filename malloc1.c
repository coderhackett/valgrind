#include <stdio.h>

int main(int argc, char **argv) {

	int *x = malloc(8*sizeof(int));

	x[9] = 0; // 数组下标越界

	free(x);
	return 0;
}
