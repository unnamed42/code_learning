#include <stdio.h>
int main(){
    int i,j,k,number;
    for(i=1;i<10;i++){
        for(j=0;j<10;j++){
            for(k=0;k<10;k++){
                number=100*i+10*j+k;
                if((i*i*i+j*j*j+k*k*k)==number)
                    printf("%d\n",number);
            }
        }
    }
    return 0;
}
            
