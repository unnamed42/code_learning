#include <stdio.h>
int int_power(int number,int n){
    int i,result=number;
    if(0==n)
        return 1;
    else{
        for(i=1;i<n;i++)
            result*=number;
        return result;
    }
}

int atoi(char* p){
    int i,length,number=0;
    for(length=0;'\0'!=*p;length++,p++)
        ;
    p-=1;
    for(i=0;i<length;i++,p--)
        number+=(*p-'0')*int_power(10,i);
    return number;
}

int main(){
    char ch[10];int number;
    printf("Please input the number:\n");
    gets(ch);
    number=atoi(ch);
    printf("After transform:\n%d",number);
    return 0;
}