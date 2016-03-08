#include <stdio.h>
void input(int m,int n,float a[][5]){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++)
			scanf("%f",&a[i][j]);
	}
}

void compute(int m,int n,float a[][5]){
	int i,j;float s=0;
	//按行加
	for(i=0;i<m-1;i++){
		for(j=0;j<n-1;j++){
			s+=a[i][j];
			a[i][n-1]=s;
		}
		s=0;
	}
	//按列加
	for(j=0;j<n-1;j++){
		for(i=0;i<m-1;i++){
			s+=a[i][j];
			a[m-1][j]=s/(m-1);
		}
		s=0;
	}
	//总平均
	for(i=0;i<m-1;i++)
		s+=a[i][n];
	a[m-1][n-1]=s/(m-1);
}

void output(int m,int n,float a[][5]){
	int i,j;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++)
			printf("%.2f ",a[i][j]);
		printf("\n");
	}
}

int main(){
	float a[4][5];
	printf("输入3x4个数据\n");
	input(3,4,a);
	compute(4,5,a);
	output(4,5,a);
	return 0;
}
