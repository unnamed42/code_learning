#include <stdio.h>
int getMax(int a[3]){
	int t=a[0],i;
	for(i=0;i<3;i++){
		if(t<a[i])
			t=a[i];
	}
	return t;
}

int getMin(int a[3]){
	int t=a[0],i;
	for(i=0;i<3;i++){
		if(t>a[i])
			t=a[i];
	}
	return t;
}

int main(){
	int a[3],i;
	printf("Please input 3 integers:\n");
	for(i=0;i<3;i++)
		scanf("%d",&a[i]);
	printf("maximum is %d\n",getMax(a));
	printf("minimum is %d\n",getMin(a));
	return 0;
}
