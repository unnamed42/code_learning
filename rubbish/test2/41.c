#include <stdio.h>
#include <string.h>
int search(char* p,char c){
    while(*p!='\0'){
        if(*p==c)
            return 1;
        p++;
    }
    return 0;
}

void del(char* p,char c){
    const int length=strlen(p);
    if(search(p,c)==0)
        printf("Not found %c in \"%s\"",c,p);
    else{
        while(search(p,c)!=0){
            for(int i=0;p[i]!='\0';i++){
                if(p[i]==c){
                    for(int j=i;j<length;j++)
                        p[j]=p[j+1];
                }
            }
        }
        printf("%s",p);
    }
}

int main(){
    char str[]="Eengggggggglish";
    printf("%s",str);
    del(str,'k');
    return 0;
}