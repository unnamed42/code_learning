#include <stdio.h>
int main(){
	char c[5];int i;
	printf("Please input 5 characters:\n");
	for(i=0;i<5;i++){
		scanf("%c",&c[i]);
		if(('W'<=c[i]&&c[i]<='Z')||('w'<=c[i]&&c[i]<='z'))
			c[i]-=22;
		else
			c[i]+=4;
	}
	for(i=0;i<5;i++)
		printf("%c",c[i]);
	return 0;
} 
