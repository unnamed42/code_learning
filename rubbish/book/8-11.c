#include <stdio.h>
void fun(int a[],int n,int * odd,int * even){
	int i;
	*odd=0;*even=0;
	for(i=0;i<n;i++){
		if(0==a[i]%2)
			*even+=a[i];
		else
			*odd+=a[i];
	}
}

int main(){
	int a[10]={1,2,3,4,5,6,7,8,9,10},oddSum,evenSum;
	fun(a,10,&oddSum,&evenSum);
	printf("sum of odd=%d,even=%d\n",oddSum,evenSum);
	return 0;
}
