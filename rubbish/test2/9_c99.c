#include <stdio.h>
int length;

void int_swap(int* a,int* b){
    int temp;
    temp=*a;*a=*b;*b=temp;
}

void insert(int* p,int d){
    int index;
    for(int i=0;i<length;i++){
        if(*(p+i)>d)
            index=i;
    }
    *(p+length)=*(p+index);*(p+index)=d;
    for(int i=index+1;i<=length;i++)
        int_swap(p+length,p+i);
}

int main(){
    int d;
    printf("Please input the length of the array:\n");
    scanf("%d",&length);
    int data[length+1];
    printf("Please input the value of the array from min to max:\n");
    for(int i=0;i<length;i++)
        scanf("%d",&data[i]);
    printf("The array you input:\n");
    for(int i=0;i<length;i++)
        printf("%d ",data[i]);
    printf("\nPlease input the integer you want to insert:\n");
    scanf("%d",&d);
    insert(data,d);
    printf("After insert:\n");
    for(int i=0;i<=length;i++)
        printf("%d ",data[i]);
}
