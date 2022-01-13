#include <stdio.h>
// 内存动态管理

int main(int argc, char **argv) {

	int i;
	char *p = (char *)malloc(10);

	char *pt = p;
	for(i = 0; i < 10; i++) {
		p[i] = 'A'+i;
	}

	free(p);

	pt[1] = 'x';

	free(pt);

	return 0;
}
