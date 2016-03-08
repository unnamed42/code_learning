#include <stdio.h>
int main(){
	int i,t=0,n,s=0,isprime=1;
	for(n=999;t<10;n-=2){
		isprime=1;
		for(i=2;i<n;i++){
			if(0==n%i){
				isprime=0;
				break;
			}
		}
		if(1==isprime){
			if(t<9){
				s+=n;t+=1;
				printf("%d+",n);
			}
			else if(9==t){
				s+=n;t+=1;
				printf("%d=%d",n,s);
			}
		}
	}
	return 0;
}
