#include <stdio.h>
#include <time.h>
int current_year(){
    struct tm* local;
    time_t t;
    t=time(NULL);
    local=localtime(&t);
    return local->tm_year+1900;
}

int main(){
    int year;
    for(year=1000;year<=current_year();year++){
        if(((year%4==0)&&(year%100!=0))||(year%400==0))
            printf("%d ",year);
    }
}