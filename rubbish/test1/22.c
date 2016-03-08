#include <stdio.h>
#include <math.h>
int main(){
    float x,y;
    printf("Please input the value of x:\n");
    scanf("%f",&x);
    if(x<5)
        y=0;
    else if(x>10)
        y=sin(x)+cos(x);
    else
        y=sin(x);
    printf("f(%.2f)=%.2f\n");
    return 0;
}
 
