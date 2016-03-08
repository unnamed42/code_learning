#include <stdio.h>
#include <stdlib.h>
typedef struct Node Node;
 struct Node
{
    int  length;
    int number;
    Node *next;
};
Node *head,*p;
void IsEmpty()
{
    if(head==NULL)
        printf("Input is wrong");
}
void InitList(int lengthm,int lengthn,int lengthp )
{
    head=(Node*)malloc(sizeof(Node));
    head->next=(Node*)malloc(sizeof(Node));
    head->length=0;
    p=head->next;
    int i;
    for(i=0;i<lengthm+lengthp+lengthn;i++)
    {
        p->next=(Node*)malloc(sizeof(Node));
        scanf("%d",&p->number);
        p->length=i+1;
        printf("%d\n",p->length);
        p=p->next;
    }
}
void print(int lengthm,int lengthn,int lengthp)
{
    p=head->next;
    int i;
    for(i=0;i<lengthm+lengthp+lengthn;i++)
    {
        printf("%d",p->number);
        p=p->next;
    }
}


void exchange(int  lengthm,int lengthn,int lengthp)
{
    p=head;
    Node* n1;Node* n2;
    Node*m;Node* x;int i;
    for(i=0;i<lengthm;i++)
    {
        p=p->next;
    }
    m=p;
    for(i=0;i<p;i++)
    {
        p=p->next;
    }
    n1=p->next;
    for(i=0;i<lengthn-1;i++)
    {
        p=p->next;
    }
    n2=p;
    head->next=x;
    head->next=n1;
    n2->next=x;
    n2->
}
int main()
{
    int m,n,np;
    Node *variable=NULL;
    scanf("%d%d%d",&m,&n,&np);
    InitList(m,n,np);
    print(m,n,np);
    exchange(m,n,np);
    IsEmpty();
    return 0;
}


