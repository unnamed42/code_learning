#include <stdio.h>
int main(){
    int n,i,minus=1;float s=0;
    printf("Please input the value of n:\n");
    scanf("%d",&n);
    for(i=1;i<=n;i++){
        s+=minus*(1/(float)i);
        minus*=-1;
    }
    printf("sum=%f\n",s);
    return 0;
}
