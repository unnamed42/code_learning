#include <stdio.h>
#include <math.h>
int main(){
    float x,y;
    printf("Please input the value of x:\n");
    scanf("%f",&x);
    if(x<1)
        y=2+cos(x);
    else if(x>=2)
        y=sqrt(x-2);
    else
        y=x*x+sin(x);
    printf("f(%.2f)=%.2f\n",x,y);
    return 0;
}
