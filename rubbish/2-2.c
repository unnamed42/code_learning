#include <stdio.h>
int main(){
	char c[5];int i;
	printf("输入五个字母\n");
	for(i=0;i<5;i++){
		c[i]=getchar();
	}
	//printf("\n");
	for(i=0;i<5;i++){
		c[i]+=4;
		printf("%c",c[i]);
	}
	return 0;
}
