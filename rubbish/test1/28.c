#include <stdio.h>
int main(){
    int x,y,solvable=0;
    for(x=0;x<10;x++){
        for(y=0;y<10;y++){
            if((10*x+y)<10||(10*y+x)<10)
                ;
            else{
                if(((10*x+y)+(10*y+x))==102)
                    printf("%d,%d\n",x,y);
                else
                    solvable+=1;
            }
        }
    }
    if(solvable==81)
        printf("无解\n");
    return 0;
}