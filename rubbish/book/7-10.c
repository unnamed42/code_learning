#include <stdio.h>
int a[10],length=sizeof(a)/sizeof(int);

int max_a(int a[]){
	int i,max=a[0];
	for(i=0;i<length;i++){
		if(max<a[i])
			max=a[i];
	}
	return max;
}

int min_a(int a[]){
	int i,min=a[0];
	for(i=0;i<length;i++){
		if(min>a[i])
			min=a[i];
	}
	return min;
}

float ave_a(int a[]){
	int i;float s=0;
	for(i=0;i<length;i++)
		s+=a[i];
	s/=(float)length;
	return s;
}

int main(){
	int i;
	printf("Please input 10 integers:\n");
	for(i=0;i<10;i++)
		scanf("%d",&a[i]);
	printf("max=%d,min=%d,average=%.2f\n",max_a(a),min_a(a),ave_a(a));
	return 0;
}
