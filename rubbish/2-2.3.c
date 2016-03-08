#include <stdio.h>
int main(){
	char c[4];int i;
	printf("输入要解密的字符串\n");
	for(i=0;i<4;i++){
		c[i]=getchar();
		if('A'<=c[i]&&c[i]<='D')
			c[i]-=22;
		else
			c[i]-=4;
		printf("%c",c[i]);
	}
	return 0;
}
