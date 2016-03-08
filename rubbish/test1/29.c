#include <stdio.h>
int main(){
    int i,f[5]={0,1},s;
    for(i=2;i<5;i++)
        f[i]=f[i-1]+f[i-2];
    for(i=0;i<5;i++)
        s+=f[i];
    printf("sum=%d\n",s);
    return 0;
}