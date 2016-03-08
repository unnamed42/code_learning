#include <stdio.h>
int day_tab[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

int leap(int year){
	//判断年份是否为闰年的函数
	return (year%4==0&&year%100!=0)||(year%400==0);
}

int sum_day(int month,int day){
	//计算日期 
	int i;
	for(i=1;i<month;i++)
		day+=day_tab[i];
	return day;
}

int main(){
	int year,month,day,days;
	printf("输入年，月，日（格式：年，月，日）\n");
	scanf("%d,%d,%d",&year,&month,&day);
	days=sum_day(month,day);//调用函数sum_day() 
	if(leap(year) && month>=3)//调用函数leap() 
		days+=1;
	printf("是该年的第%d天",days);
}