#include <stdio.h>
int main(){
    float m=1,n=2,temp,s=0;int i;
    for(i=1;i<=10;i++){
        s+=m/n;
        temp=m;m=n;n+=temp;
    }
    printf("sum=%f\n",s);
    return 0;
}
