#include <stdio.h>
int gcd(int a,int b){
	int t;
	if(a<b){
		t=a;a=b;b=t;
	}
	while(0!=a%b){
		t=a%b;a=b;b=t;
	}
	return t;
}

int mul(int a,int b){
	return a*b/gcd(a,b);
}

int main(){
	int m,n;
	printf("Please input 2 integers(format:a,b):\n");
	scanf("%d,%d",&m,&n);
	printf("the greatest common divisor of %d and %d is %d\n",m,n,gcd(m,n));
	printf("the lowest common multiple of %d and %d is %d\n",m,n,mul(m,n));
}
