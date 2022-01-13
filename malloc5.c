#include <stdio.h>

int main(int argc, char **argv) {

	char *p = malloc(1);

	*p = 'a';

	char c = *p; // 地址加1

	printf("[%c]\n",c); 
	
	free(p);// 内存多次释放
	free(p);
	free(p);

	return 0;
}
