#include <stdio.h>
int IsSameRowSum(int a[][]){
	int n=sizeof(a)/sizeof(a[0]),i,j;
	int s[n-1]={};
	for(i=0;i<n-1;i++){
		for(j=0;j<n-1;j++){
			s[i]+=a[i][j];
		}
	}
	for(i=0;i<n-2;i++){
		if(s[i]!=s[i+1])
			return 0;
	}
	return 1;
}

int IsSameColumnSum(int a[][]){
	int n=sizeof(a)/sizeof(a[0]),i,j;
	int s[n-1]={};
	for(i=0;i<n-1;i++){
		for(j=0;j<n-1;j++){
			s[i]+=a[i][j];
		}
	}
	for(i=0;i<n-2;i++){
		if(s[i]!=s[i+1])
			return 0;
	}
	return 1;
}

int IsSameDiagonalSum(int a[][]){
	int n=sizeof(a)/sizeof(a[0]),i,j;
	int s1[n-1]={},s2[n-1]={};
	for(i=0;i<n-1;i++){
		s1[i]+=a[i][i];
		s2[i]+=a[i][n-i-1];
	}
	for(i=0;i<n-2;i++){
		if(s[i]!=s[i+1])
			return 0;
	}
	return 1;
}

int IsNoRepeat(int a[][]){
	;//TODO
}

int main(){
	int n;
	printf("Please input an integer n:\n");
	scanf("%d",&n);
	int a[n][n];
	printf("Please input the elements of the nxn matrix:\n");
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++)
			scanf("%d",&a[i][j]);
	}
	if(IsNoRepeat(a)==1&&IsSameDiagonalSum(a)==1&&IsSameColumnSum(a)==1&&IsSameRowSum(a)==1)
		printf("It's a magic square.\n");
	else
		printf("It's not a magic square.\n");
	return 0;
}