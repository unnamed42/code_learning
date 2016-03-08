#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NEWPTR(type) (type*)malloc(sizeof(type))
typedef struct{
    int time;
    int id;
    int bus;
}Type;
typedef struct node{
    Type data;
    struct node *next;
    struct node *prev;
}node;
typedef struct queue{
    node *front;
    node *rear;
}queue;

queue *init(){
    queue *q=NEWPTR(queue);
    q->front=q->rear=NULL;
    return q;
}
queue *create(int length){
    queue *q=NEWPTR(queue);
    q->front=NEWPTR(node);
    q->front->prev=NULL;
    node *temp=q->front;
    for(;length>1;length--){
        temp->next=NEWPTR(node);
        temp->next->prev=temp;
        temp=temp->next;
    }
    temp->next=NULL;
    q->rear=temp;
    return q;
}
void destroy(queue *q){
    node *save,*temp=q->front;
    while(temp!=NULL){
        save=temp->next;
        free(temp);
        temp=save;
    }
    q->front=q->rear=NULL;
}
void en_queue(queue *q,Type *elem){
    if(q->front==NULL){
        q->front=q->rear=NEWPTR(node);
        q->front->data=*elem;
        q->front->next=q->front->prev=NULL;
    }
    else{
        node *temp=NEWPTR(node);
        temp->data=*elem;
        temp->next=q->front;
        temp->next->prev=temp;
        temp->prev=NULL;
        q->front=temp;
    }
}
void de_queue(queue *q,Type *elem){
    if(q->rear==NULL)
        perror("Error while accessing rear pointer!\n");
    *elem=q->rear->data;
    if(q->rear==q->front){
        free(q->rear);
        q->front=q->rear=NULL;
    }
    node *save=q->rear->prev;
    save->next=NULL;
    free(q->rear);
    q->rear=save;
}
int queue_length(queue *q){
    node *temp=q->front;
    int i=0;
    while(temp!=NULL){
        i++;
        temp=temp->next;
    }
    return i;
}
int empty(queue *q){
    return q->front==NULL;
} 

void swap(Type *a,Type *b){
    Type t;
    t.time=a->time;t.id=a->id;t.bus=a->bus;
    a->time=b->time;a->id=b->id;a->bus=b->bus;
    b->time=t.time;b->id=t.id;b->bus=t.bus;
}
void bubble_sort(Type *a,int length){
    for(int i=0;i<length-1;i++){                               
        for(int j=0;j<length-i-1;j++){ 
            if(a[j].time>a[j+1].time)
                swap(a+j,a+j+1); 
        }
    }
}

void generate(Type *elem){
    static int i=0;
    srand(time(NULL));
    elem->id=i++;
    elem->bus=(rand()+1)%4;
    elem->time=rand()%940;
}

int main(void){
    Type passenger[5];queue *q=init();
    for(int i=0;i<5;i++)
        generate(passenger+i);
    bubble_sort(passenger,5);
    for(int i=0;i<5;i++)
        en_queue(q,passenger+i);
    for(int i=0;i<5;i++){
        Type p;
        de_queue(q,&p);
        printf("%d %d\n",p.id,p.bus);
    }
    return 0;
} 
