#include <stdio.h>
int main(){
    float f;
    printf("Please input a real number:\n");
    scanf("%f",&f);
    f=((int)(f*100+0.5))/100.0
    printf("%f",f);
    return 0;
}