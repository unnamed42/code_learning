#include <stdio.h>
#define MAX 10
void print_array(int n,int a[][MAX]){
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++)
			printf("%2d ",a[i][j]);
		printf("\n");
	}
}

int main(){
	int n,i,j,maxr=0,minr=0,temp,a[MAX][MAX];
	printf("输入nxn数组的n值\n");
	scanf("%d",&n);
	printf("输入%dx%d数组的值\n",n,n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++)
			scanf("%d",&a[i][j]);
	}
	printf("所输入的数组是\n");
	print_array(n,a);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(a[i][maxr]<a[i][j])
				maxr=i;
			if(a[i][minr]>a[i][j])
				minr=i;
		}
	}
	if(maxr==minr)
		;
	else{
		for(j=0;j<n;j++){
			temp=a[maxr][j];a[maxr][j]=a[minr][j];a[minr][j]=temp;
		}
	}
	printf("交换后的数组是\n");
	print_array(n,a);
	return 0;
}
