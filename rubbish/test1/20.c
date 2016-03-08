#include <stdio.h>
int max(int a,int b,int c){
    if(a<b)
        a=b;
    if(a<c)
        a=c;
    return a;
}
 
int min(int a,int b,int c){
    if(a>b)
        a=b;
    if(a>c)
        a=c;
    return a;
}

int main(){
    int a,b,c;
    printf("Please input 3 integers(format: a,b,c):\n");
    scanf("%d,%d,%d",&a,&b,&c);
    printf("max=%d,min=%d\n",max(a,b,c),min(a,b,c));
    return 0;
}