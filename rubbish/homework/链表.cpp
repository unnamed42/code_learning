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
        //printf("%d\n",p->length);
        p=p->next;
    }
}
void print(int lengthm,int lengthn,int lengthp)
{
    p=head->next;
    int i;
    for(i=0;i<lengthm+lengthp+lengthn;i++)
    {
        printf("%d ",p->number);
        //printf("\n");
        p=p->next;
    }
}
void exchange(int  lengthm,int lengthn,int lengthp)
{
    p=head;Node* falsep;
    Node* m;Node* n;Node* fp;
    int i;
    for(i=0;i<lengthm;i++)
    {
        p=p->next;
    }
    m=p;
   // printf("%d ",m->number);
    for(i=0;i<lengthp;i++)
    {
        p=p->next;
    }
    fp=p;
    //printf("%d ",fp->number);
    for(i=0;i<lengthn;i++)
    {
        p=p->next;
    }
    n=p;
    //printf("%d\n",n->number);
    p=head->next;
    head->next=fp->next;
    n->next=m->next;
    fp->next=p;
    m->next=NULL;
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
    printf(" ");
    print(m,n,np);
    return 0;
}

