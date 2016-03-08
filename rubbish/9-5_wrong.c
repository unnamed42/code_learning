#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Student{
	int number;
	char name[10];
	char sex[8];
	int age;
	char subject[15];
	struct Student* next;
} Student;

void input(Student* stu){
	printf("学号:");
	scanf("%d",&stu->number);
	printf("姓名:");
	scanf("%s",stu->name);
	printf("性别:");
	scanf("%s",stu->sex);
	printf("年龄:");
	scanf("%d",&stu->age);
	printf("专业:");
	scanf("%s",stu->subject);
}

Student* CreateList(int n){
	Student *start,**temp;int i;
	start=(Student*)malloc(sizeof(Student));
	start->next=NULL;
	temp=&start->next;
	for(i=0;i<n-1;i++){
		*temp=(Student*)malloc(sizeof(Student));
		input(*temp);
		temp=&(*temp)->next;
	}
	(*temp)=NULL;
	return start;
}

void RemoveSubject(Student* stu,char* subject){
	int i;Student* temp;
	while(stu->next!=NULL){
		if(strcmp(stu->subject,subject)==0){
			temp=stu->next;
			free(stu);
			stu=temp;
		}
		stu=stu->next;
	}
}

void printList(Student* stu){
	while(stu->next!=NULL){
		printf("学号:%d\n",stu->number);
		printf("姓名:%s\n",stu->name);
		printf("性别:%s\n",stu->sex);
		printf("年龄:%d\n",stu->age);
		printf("专业:%s\n\n",stu->subject);
		stu=stu->next;
	}
}

int main(){
	Student* start;int n;char subject[15];
	printf("学生人数:");
	scanf("%d",&n);
	start=CreateList(n);
	//printf("输入要删除的专业:");
	//scanf("%s",subject);
	//RemoveSubject(start,subject);
	printList(start);
	return 0;
}
