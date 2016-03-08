#include <stdio.h>
int main(){
    float m=1,n=2,temp,s=0;int i,minus=1;
    for(i=1;i<=10;i++){
        s+=minus*(n/m);
        temp=m;m=n;n+=temp;
        minus*=-1;
    }
    printf("sum=%f\n",s);
    return 0;
}

