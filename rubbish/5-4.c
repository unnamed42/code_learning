#include <stdio.h>
#include <math.h>
int main(){
	int n,accuracy;float pi=0;
	printf("输入精度:\n");
	scanf("%d",&accuracy);
	for(n=1;n<=accuracy;n++)
		pi+=1.0/(n*n);
	pi*=6;pi=sqrt(pi);
	printf("pi=%f",pi);
	return 0;
}
