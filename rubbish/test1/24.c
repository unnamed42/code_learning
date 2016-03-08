#include <stdio.h>
/*int prime_tab[100]={2};

void gen_tab(int n){
    int i,j,t=1,isprime;
    for(j=3;j<=n;j++){
        isprime=1;
        for(i=2;i<j;i++){
            if(j%i==0){
                isprime=0;
                break;
            }
        }
        if(1==isprime){
            prime_tab[t]=j;
            t++;
        }
    }
}        
*/

int main(){
    int n,m=0,i;
    printf("请输入n:\n");
    scanf("%d",&n);
    for(i=1;i<n;i++){
        if(n%i==0)
            m+=i;
    }
    if(m==n)
        printf("是完数\n");
    else
        printf("不是完数\n"); 
    return 0;
}