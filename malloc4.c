#include <stdio.h>

int main(int argc, char **argv) {

	char *p = malloc(1);

	*p = 'a';

	char c = *p; // 地址加1

	printf("[%c]\n",c); // 申请后没有释放p 内存泄漏


	return 0;
}
