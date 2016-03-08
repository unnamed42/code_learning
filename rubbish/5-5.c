#include <stdio.h>
int main(){
	int daxie=0,xiaoxie=0,qita=0,shuzi=0,i;char c[10];
	printf("输入10个字符\n");
	for(i=0;i<10;i++)
		c[i]=getchar();
	for(i=0;i<10;i++){
		if('0'<=c[i]&&c[i]<='9')
			shuzi+=1;
		else if('A'<=c[i]&&c[i]<='Z')
			daxie+=1;
		else if('a'<=c[i]&&c[i]<='z')
			xiaoxie+=1;
		else
			qita+=1;
	}
	printf("数字=%d,大写字母=%d,小写字母=%d,其他字符=%d",shuzi,daxie,xiaoxie,qita);
	return 0;
}
