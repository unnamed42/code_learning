#include <stdio.h>
int main(){
    int n;
    printf("请输入一个三位正整数:\n");
    scanf("%d",&n);
    if((n/100)==(n%10))
        printf("是回文数\n");
    else
        printf("不是回文数\n");
    return 0;
}
