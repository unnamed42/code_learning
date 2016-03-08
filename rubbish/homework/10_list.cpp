#include <iostream>
#include <cstdlib>

struct node{
    int data;
    node *prev;
    node *next;
    node():data(0),prev(NULL),next(NULL){}
    node(int i):data(i),prev(NULL),next(NULL){}
    node(node *p):data(p->data),prev(p->prev),next(p->next){}
};

class list{
    private:
        node *head;
    public:
        list():head(NULL){}
        list(int length);
        ~list();
        node *get_end();
        void print();
        void exchange(int m,int n);
};
list::list(int length){
    node *temp;
    head=new node();
    head->prev=NULL;
    srand(1);
    temp=head;
    for(;length>0;length--){
        temp->next=new node(rand()%10);
        temp->next->prev=temp;
        temp=temp->next;
    }
    temp->next=NULL;
}
list::~list(){
    node *save;
    while(head!=NULL){
        save=head->next;
        delete head;
        head=save;
    }
}
node* list::get_end(){
    node *p=head;
    while(p->next!=NULL)
        p=p->next;
    return p;
}
void list::print(){
    node *temp=head->next;
    while(temp!=NULL){
        std::cout<<temp->data<<" ";
        temp=temp->next;
    }
    std::cout<<std::endl;
}
void list::exchange(int m,int n){
    node *node_m_start,*node_m_end,*node=head,*end=get_end();
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

int main(void){
    list head(10);
    head.print();
    head.exchange(1,7);
    head.print();
    return 0;
}  
