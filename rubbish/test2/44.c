#include <stdio.h>
int main(){
    int a,b,c;
    for(a=1;a<=9;a++){
        for(b=0;b<=9;b++){
            for(c=1;c<=9;c++){
                if((c+10*b+100*a+a+10*b+100*c)==1333)
                    printf("%d %d %d\n",a,b,c);
            }
        }
    }
    return 0;
} 
