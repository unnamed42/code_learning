#include <stdio.h>
long fib(int n){
    if(0==n||1==n)
        return n;
    else if(n<0)
        return -1;
    else{
        long a=0,b=1,c;
        for(int i=0;i<n-1;i++){
            c=a+b;
            a=b;b=c;
        }
        return c;
    }
}

int main(){
    int n;long sum=0;
    printf("Please input an integer:");
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        sum+=fib(i);
        printf("%ld+",fib(i));
    }
    printf("\b=%ld",sum);
    return 0;
} 
