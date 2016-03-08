#include <stdio.h>
int main(){
	int m,n,orig_m,orig_n,t;
	printf("输入m,n的值(格式m,n)\n");
	scanf("%d,%d",&m,&n);
	orig_n=n,orig_m=m;
	if(m<n){
		m=orig_n;n=orig_m;
	}
	while(0!=m%n){
		t=m%n;m=n;n=t;
	}
	printf("%d与%d的最大公约数是%d，最小公倍数是%d",orig_m,orig_n,n,orig_m*orig_n/n);
	return 0;
}
