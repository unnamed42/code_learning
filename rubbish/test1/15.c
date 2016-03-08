#include <stdio.h>
int main(){
    char ch;
    printf("Please input a char:\n");
    ch=getchar();
    if(ch>='A'&&ch<='Z')
        ch+=32;
    else if(ch>='a'&&ch<='z')
        ch-=32;
    else
        ;
    printf("ch=%c\n",ch);
    return 0;
}