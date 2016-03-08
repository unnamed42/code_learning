#include <stdio.h>
#include <stdlib.h>
void bubble_sort(int a[],int length){
	int i,j,temp;
	for(i=0;i<length-1;i++){
		for(j=0;j<length-1-i;j++){
			if(a[j]>a[j+1]){
				temp=a[j];a[j]=a[j+1];a[j+1]=temp;
			}
		}
	}
} 

int mid_search(int a[],int element,int length){
	int max=length-1,min=0,mid;
	while(max>=min){
		mid=(max+min)/2;
		if(element==a[mid])
			return mid;
		else if(element>a[mid])
			min=mid+1;
		else
			max=mid-1;
	}
	return -1;
}

int main(){
	int a[20],i,element,index;
	for(i=0;i<20;i++)
		a[i]=rand()%10;
	bubble_sort(a,20);
	printf("Please input the element to search:\n");
	scanf("%d",&element);
	index=mid_search(a,element,20);
	if(-1==index)
		printf("not found %d\n",element);
	else
		printf("%d is located at %d",element,index+1);
	return 0;
}