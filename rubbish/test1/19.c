#include <stdio.h>
int main(){
    char currency;int money;
    printf("Please input the price:\n");
    scanf("%1c%d",&currency,&money);
    switch(currency){
        case '$':
            money*=6;
            break;
        case 'E':
            money*=8;
            break;
        default :
            printf("Error!\n");
            break;
    }
    currency='Y';
    printf("price=%c%d\n",currency,money);
    return 0;
}
