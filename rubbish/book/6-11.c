#include <stdio.h>
void transpose_5x5(int a[5][5],int b[5][5]){
	int i,j,temp;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			b[j][i]=a[i][j];
		}
	}
}

void input_5x5(int a[5][5]){
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++)
			scanf("%d",&a[i][j]);
	}
}

void print_5x5(int a[5][5]){
	int i,j;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++)
			printf("%d ",a[i][j]);
		printf("\n");
	}
}

int main(){
	int a[5][5],b[5][5];
	printf("Please input the elements of the 5x5 matrix\n");
	input_5x5(a);
	printf("The matrix you input is:\n");
	print_5x5(a);
	transpose_5x5(a,b);
	printf("The transpose of the matrix is:\n");
	print_5x5(b);
	return 0;
}
