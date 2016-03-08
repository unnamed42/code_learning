#include <stdio.h>
int main(){
    int n,i,j,s;float sum=0;
    printf("Please input the value of n:\n");
    scanf("%d",&n);
    for(i=1;i<=n;i++){
        s=0;
        for(j=1;j<=i;j++)
                s+=j;
        sum+=1/(float)s;
    }
    printf("sum=%.6f\n",sum);
    return 0;
}