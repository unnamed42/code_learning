#include <stdio.h>
int main(){
    int a[3],n;
    scanf("%d",&n);
    for(int i=2;i>=0;i--){
        a[i]=n%10;
        n/=10;
    }
    printf("%d %d %d",a[0],a[1],a[2]);
    return 0;
} 
