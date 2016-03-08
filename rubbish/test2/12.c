#include <stdio.h>
int my_remove(int* p,int d){
    int i;
    for(i=0;i<6;i++,p++){
        if(*p==d)
            break;
    }
    if(i<6){
        printf("Not found %d\n",d);
        return 0;
    }
    else{
        *p=0;p++;
        for(;i<6;i++,p++){
            *p=*(p+1);*(p+1)=0;
        }
    }
    return 1;
}

int main(){
    int data[6]={3,23,-7,8,9,10},i,d,isfound;
    printf("Origin array is:\n");
    for(i=0;i<6;i++)
        printf("%d ",data[i]);
    printf("\nPlease input the integer you want to delete:\n");
    scanf("%d",&d);
    isfound=my_remove(data,d);
    if(1==isfound){
        printf("After deletion:\n");
        for(i=0;i<6;i++){
            if(0!=data[i])
                printf("%d ",data[i]);
        }
    }
    return 0;
}
