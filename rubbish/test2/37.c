#include <stdio.h>
void nstrcat(char* t,char* s){
    for(;'\0'!=*t;t++)
        ;
    //t-=1;
    for(;'\0'!=*s;t++,s++){
        if('0'<=*s&&*s<='9')
            *t=*s;
        else
            t-=1;
    }
}

int main(){
    char str1[30]={"WRHTEshh22"},str2[20]={"125etahe2"};
    printf("Original str1=%s,str2=%s\n",str1,str2);
    nstrcat(str1,str2);
    printf("After:\nstr1=%s\n",str1);
    return 0;
}