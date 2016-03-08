#include<stdio.h>
#include<stdlib.h>
#define LIST_INIT_SIZE 1000
#define LISTINCREMENT  10
#define ElemType int
typedef struct
{
	ElemType *elem;
	//int lengthm;
	//int lengthn;
	int length;
	int listzize;
}SqList;
void create(SqList *p,int lengthmn)
{
	printf("input  %d datas",lengthmn);
    p->elem=(ElemType*)malloc(sizeof(ElemType)*(lengthmn));
	int i;
	for(i=0;i<lengthmn;i++)
	{
		scanf("%d",p->elem+i);
	}
	 p->length=i+1;
	p->length=lengthmn;
}
void exchange(SqList *p,int m,int n)
{
    int j,i;
	if(n<1 || m<1 || (m+n>p->length))return;
	SqList q;
	q.elem=(ElemType*)malloc(sizeof(ElemType)*(m+n));
	for(i=0;i<(m+n);i++)
	{
		q.elem[i]=p->elem[i];
	}
	for(i=0;i<n;i++)
	{
		p->elem[i]=q.elem[m+i];
	}
	for(i=m;i<(m+n);i++)
	{
	 	p->elem[i]=q.elem[i-m];
	}
}
void print(SqList *p)
{
	int i;
	for(i=0;i<p->length;i++)
	{
		printf("%d ",p->elem[i]);
	}
}
int main()
{
    int m,n;SqList list;
	scanf("%d%d",&m,&n);
	create(&list,m+n);
	print(&list);
	exchange(&list,m,n);
	print(&list);
	return 0;
}
