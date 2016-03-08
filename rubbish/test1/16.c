#include <stdio.h>
int main(){
    int a,b,c;
    printf("Please input 3 integers(format: a,b,c):\n");
    scanf("%d,%d,%d",&a,&b,&c);
    if(a<b)
        a=b;
    if(a<c)
        a=c;
    printf("max of them is %d\n",a);
    return 0;
}
