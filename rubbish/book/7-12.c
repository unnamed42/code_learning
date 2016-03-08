#include <stdio.h>
int encrypt(int number){
	int a[4],i,temp;
	for(i=0;i<4;i++){
		a[i]=(number%10+5)%10;
		number/=10;
	}
	temp=a[0];a[0]=a[3];a[3]=temp;
	temp=a[1];a[1]=a[2];a[2]=temp;
	return a[3]*1000+a[2]*100+a[1]*10+a[0];
}

int main(){
	int integer;
	printf("Please input a number between 1000 and 9999:\n");
	scanf("%d",&integer);
	printf("Encryption is %d\n",encrypt(integer));
	return 0;
}
