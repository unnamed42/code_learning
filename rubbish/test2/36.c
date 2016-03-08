#include <stdio.h>
#include <string.h>
void mstrcpy(char* t,char* s,int m){
    int length_t=strlen(t),length_s=strlen(s),i;
    s+=(m-1);
    for(i=0;i<(length_s-m);i++)
        *(s+i)='\0';
    for(i=0;i<length_t;i++)
        *(s+i)=*(t+i);
}

int main(){
    char str1[10]={"gwr22"},str2[30]={"sBhteahne"};int index;
    printf("Please give the index from which you'd like to copy:\n");
    scanf("%d",&index);
    printf("original str2=%s\n",str2);
    mstrcpy(str1,str2,index);
    printf("After:\nstr1=%s\nstr2=%s\n",str1,str2);
    return 0;
}