#include <stdio.h>

int main(int argc, char **argv) {

	char *p = malloc(1);

	*p = 'a';

	char c = *(p+1); // 地址加1

	printf("[%c]\n",c);

	free(p); // 释放

	//c = *p; // 取值

	return 0;
}
