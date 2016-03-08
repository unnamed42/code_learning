#include <stdio.h>
void itoa(int n,char* str){
    int i,minus=0;
    if(n<0){
        minus=1;
        n*=-1;
    }
    for(i=0;n!=0;i++){
        str[i]=n%10+'0';
        n/=10;
    }
    if(1==minus){
        str[i]='-';
        i++;
    }
    for(int j=0;j<i/2;j++){
        char temp;
        temp=str[j];str[j]=str[i-j-1];str[i-j-1]=temp;
    }
}

int main(){
    int n;char str[10]="";
    scanf("%d",&n);
    myitoa(n,str);
    printf("%s",str);
    return 0;
}