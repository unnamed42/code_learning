#include <stdio.h>
int fun(char* p){
    int i,length,not=0;
    for(length=0;'\0'!=*p;length++,p++)
        ;
    char str[length];
    for(i=0;i<length;p--,i++)
        str[i]=*p;
    for(i=0;i<length;i++){
        if(str[i]==*(p+i))
            not=0;
        else
            not=1;
    }
    return not;
}

int main(){
    printf("Please input a string:\n");
    char str[10];
    scanf("%s",str);
    if(fun(str)==0)
        printf("no\n");
    else
        printf("%s",str);
    return 0;
}