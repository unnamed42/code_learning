#include <stdio.h>
#include <stdlib.h>

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
    printf("input  %d datas:",lengthmn);
    p->elem=(ElemType*)malloc(sizeof(ElemType)*(lengthmn));
    int i;
    for(i=0;i<lengthmn;i++)
    {
        //scanf("%d",p->elem+i);
        p->elem[i]=rand()%16;
    }
    p->length=lengthmn;
}

void exchange(SqList *p,int m,int n)
{
    int j,i;int data;
    if(n<1 || m<1 || (m+n>p->length))return;
    for(j=0;j<n;j++)
    {
        for(i=m+j;i>=1+j;i--)
        {
            data=p->elem[i-1];
            p->elem[i-1]=p->elem[i];
            p->elem[i]=data;
        }
    }
}

void print(SqList *p)
{
    int i;
    for(i=0;i<p->length;i++)
    {
        printf("%d ",p->elem[i]);
    }
    printf("\n");
}

int main()
{	
    int m,n;SqList list;
    /* 搞个输入提示吧 */
    printf("Input m, n:");
    scanf("%d%d",&m,&n);
    create(&list,m+n);
    print(&list);
    exchange(&list,m,n);
    print(&list);
    return 0;
}
