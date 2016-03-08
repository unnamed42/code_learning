#include <stdio.h>
int main(){
    char ch;
    printf("Please input a char:\n");
    ch=getchar();
    switch(ch){
        case 'A':
            printf("95\n");
            break;
        case 'B':
            printf("85\n");
            break;
        case 'C':
            printf("75\n");
            break;
        case 'D':
            printf("65\n");
            break;
        case 'E':
            printf("65\n");
            break;
        default:
            printf("error!\n");
            break;
    }
    return 0;
}
    
