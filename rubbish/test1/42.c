#include <stdio.h>
int main(){
    int n,up,down;
    printf("Input an integer with 3 digits:");
    scanf("%d",&n);
    down=(n/100)*100;
    up=down+99;
    printf("up=%d,down=%d",up,down);
    return 0;
} 
