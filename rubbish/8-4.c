#include <stdio.h>
void swap(int *a,int *b){
	int temp;
	temp=*a;*a=*b;*b=temp;
}

void sort(int* p,int length){
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<2-i;j++){
			if(*(p+j)>*(p+j+1))
				swap(p+j,p+j+1);
		}
	}
}

int main(){
	int a[3],i;
	printf("Please input 3 integers:\n");
	for(i=0;i<3;i++)
		scanf("%d",&a[i]);
	sort(a,3);
	printf("After sorting:\n");
	for(i=0;i<3;i++)
		printf("%d ",a[i]);
	return 0;
}
