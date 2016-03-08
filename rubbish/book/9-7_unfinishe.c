#include <stdio.h>
typedef struct{
	int number;
	char name[10];
	int score[3];
} Student;

void input(Student* stu){
	printf("学号:");
	scanf("%d",&stu->number);
	printf("姓名:");
	scanf("%s",stu->name);
	printf("高数:");
	scanf("%d",&stu->score[0]);
	printf("英语:");
	scanf("%d",&stu->score[1]);
	printf("程序设计:");
	scanf("%d",&stu->score[2]);

}

void score_sum(Student stu[],int length,int score[]){
	int i;
	for(i=0;)
}

void sort_and_print(Student stu[],int length){
	int order[length];int i;
	for(i=0;i<length;i++)
		order[i]=stu[i].number;
}

void print(Student* stu){
	printf("学号:%d\n",stu->number);
	printf("姓名:%s\n",stu->name);
	printf("高数:%d\n英语:%d\n程序设计:%d\n",stu->score[0],stu->score[1],stu->score[2]);
}

int main(){
	Student stu[10];int i;
	for(i=0;i<10;i++)
		input(&stu[i]);

}
