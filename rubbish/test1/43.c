#include <stdio.h>
int main(){
    float n,up,down;
    printf("Input a pure demical with 3 digits:");
    scanf("%f",&n);
    down=((int)(n*10))/10.0;
    up=down+0.099;
    printf("up=%f,down=%f",up,down);
    return 0;
} 
