#include <stdio.h>
#include <math.h>
int main(){
    float x,y;
    printf("Please input the value of x:\n");
    scanf("%f",&x);
    if(x<6)
        y=1+x;
    else if(x>=10)
        y=sin(x)+2*x;
    else
        y=sqrt(x-2)+1;
    printf("f(%.2f)=%.2f\n");
    return 0;
}
