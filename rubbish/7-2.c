#include <stdio.h>
void primeNum(int n){
	int i,isprime=1;
	for(i=2;i<n;i++){
		if(0==n%i){
			isprime=0;
			break;
		}
	}
	if(1==isprime)
		printf("%d is a prime number\n",n);
	else
		printf("%d is not a prime number\n",n);
}

int main(){
	int x;
	printf("Please input an integer:\n");
	scanf("%d",&x);
	primeNum(x);
	return 0;
}
