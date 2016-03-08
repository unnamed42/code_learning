#include <stdio.h>
typedef struct{
	char name[10];
	int age;
}People;

int middle(int a,int b,int c){
	int t;
	if(a>b){
		t=a;a=b;b=t;
	}
	if(b>c){
		t=b;b=c;c=t;
	}
	if(a>b){
		t=a;a=b;b=t;
	}
	return b;
}

int main(){
	People man[3]={{{"a"},15},{{"b"},30},{{"c"},10}};int index,i;
	index=middle(man[0].age,man[1].age,man[2].age);
	for(i=0;i<3;i++){
		if(man[i].age==index){
			index=i;
			break;
		}
	}
	printf("name:%s\nage:%d\n",man[index].name,man[index].age);
	return 0;
}

