#include <stdio.h>
int isprime(int n){
    int i;
    for(i=2;i<n;i++){
        if(n%i==0)
            return 0;
    }
    return 1;
}

int main(){
    int n;
    printf("Please input the value of n:\n");
    scanf("%d",&n);
    switch(isprime(n)){
        case 0:
            printf("not prime\n");
            break;
        case 1:
            printf("is prime\n");
            break;
    }
    return 0;
}