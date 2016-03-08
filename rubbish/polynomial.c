#include <stdio.h>
#include <stdlib.h>

#define NEWPTR(type) (type*)malloc(sizeof(type))

typedef struct p{
    float coef;
    int expn;
    struct p *next;
} polynomial;

polynomial *init(){
    polynomial *head=NEWPTR(polynomial);
    head->coef=0;head->expn=0;head->next=NULL;
    return head;
}

polynomial *copy(polynomial *node){
    polynomial *p=NEWPTR(polynomial);
    p->coef=node->coef;
    p->expn=node->expn;
    p->next=NULL;
    return p;
}

void destroy(polynomial *head){
    polynomial *save;
    while(head!=NULL){
        save=head->next;
        free(head);
        head=save;
    }
}

void input(polynomial *head){
    polynomial **temp;float co=0;int ex=0;
    temp=&head->next;
    printf("请输入数据(格式：系数 指数)，按指数从小到大顺序输入\n");
    while(1){
        scanf("%f %d",&co,&ex);
        if(co==0&&ex==0)
            break;
        *temp=NEWPTR(polynomial);
        if(*temp==NULL){
            perror("Error!\n");
            return;
        }
        (*temp)->coef=co;(*temp)->expn=ex;
        temp=&(*temp)->next;
    }
    *temp=NULL;
}

void print(polynomial *head){
    head=head->next;
    while(head!=NULL){
        if(head->coef>0)
            printf("+");
        printf("%.2fx^%d",head->coef,head->expn);
        head=head->next;
    }
    printf("\n");
}

void manipulate(polynomial *p1,polynomial *p2,char operation){
    p2=p2->next;
    while(p1->next!=NULL&&p2!=NULL){
        if(p2->expn > p1->next->expn)
            p1=p1->next;
        else if(p2->expn < p1->next->expn){
            polynomial *temp=copy(p2);
            temp->next=p1->next;
            p1->next=temp;
            p1=temp;
            p2=p2->next;
        }
        else{
            switch(operation){
                case '+':
                    p1->next->coef+=p2->coef;
                    break;
                case '-':
                    p1->next->coef-=p2->coef;
                    break;
                default:
                    perror("No such operation.\n");
                    return;
            }
            p1=p1->next;
            p2=p2->next;
        }
    }
    if(p1->next==NULL){//this is why I use p1->next to replace p1 above.
        while(p2!=NULL){
            polynomial *temp=copy(p2);
            if(operation=='-')
                temp->coef*=-1;
            temp->next=p1->next;
            p1->next=temp;
            p1=temp;
            p2=p2->next;
        }
    }
}

int main(){
    polynomial *p1=init(),*p2=init();
    input(p1);
    input(p2);
    print(p1);
    print(p2);
    manipulate(p1,p2,'+');
    print(p1);
    destroy(p1);
    destroy(p2);
} 
