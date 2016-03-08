#include <stdlib.h>
#include <stdio.h>

#define NEWPTR(type) (type*)malloc(sizeof(type))

typedef struct l{
    int data;
    struct l *next;
    struct l *prev;
} list;

list *create(int length){
    list *head,*node;
    head=NEWPTR(list);
    head->prev=NULL;
    srand(1);
    node=head;
    for(;length>0;length--){
        node->next=NEWPTR(list);
        node->next->prev=node;
        node=node->next;
        node->data=rand()%10;
    }
    node->next=NULL;
    return head;
}

list *getEnd(list *head){
    while(head->next!=NULL&&head!=NULL)
        head=head->next;
    return head;
}

void exchange(list *head,int m,int n){
    list *node_m_start,*node_m_end,*node=head,*end=getEnd(head);
    node=node->next;
    node_m_start=node;
    for(;m>1;m--){
        if(node==NULL||node->next==NULL)
            return;
        node=node->next;
    }
    node_m_end=node;
    end->next=node_m_end->next;
    end->next->prev=end;
    for(;n>1;n--)
        end=end->prev;
    end->prev->next=node_m_start;
    node_m_start->prev=end->prev;
    node_m_end->next=NULL;
    head->next=end;
    head->next->prev=head;
}

void print(list *head){
    head=head->next;
    while(head!=NULL){
        printf("%d ",head->data);
        head=head->next;
    }
    printf("\n");
}

int main(void){
    list *head=create(10);
    print(head);
    exchange(head,2,2);
    print(head);
} 
