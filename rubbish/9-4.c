#include <stdio.h>
typedef struct{
	int number;
	char name[10];
	int score[3];
	float ave;
} Student;

void aveScore(Student* stu){
	stu->ave=(stu->score[0]+stu->score[1]+stu->score[2])/3.0;
}

void input(Student* stu){
	printf("学号:");
	scanf("%d",&stu->number);
	printf("姓名:");
	scanf("%s",stu->name);
	printf("programming 成绩:");
	scanf("%d",&stu->score[0]);
	printf("database 成绩:");
	scanf("%d",&stu->score[1]);
	printf("network 成绩:");
	scanf("%d",&stu->score[2]);
}

void print(Student* stu){
	printf("姓名:%s ",stu->name);
	printf("平均成绩:%.2f\n",stu->ave);
}

int main(){
	Student stu[6];int i;
	for(i=0;i<6;i++){
		input(&stu[i]);
		aveScore(&stu[i]);
	}
	for(i=0;i<6;i++)
		print(&stu[i]);
	return 0;
}