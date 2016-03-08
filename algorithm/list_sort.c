typedef struct l{
    int data;
    struct l *next;
} list;

/*list *create(int n){
    list *h,**t;
    srand(1);
    h=(list*)malloc(sizeof(list));
    h->data=rand()%30;
    t=&h->next;
    for(;n>1;n--){
        *t=(list*)malloc(sizeof(list));
        (*t)->data=rand()%30;
        t=&(*t)->next;
    }
    *t=NULL;
    return h;
}*/

list *insert_sort(list *head){
    if(head==NULL)
        return NULL;
    list *node=head->next;
    head->next=NULL;
    while(node!=NULL){
        list *save=node->next;
        if(node->data<head->data){
            node->next=head;
            head=node;
        }
        else{
            list *temp=head;
            while(temp->next!=NULL && temp->next->data<node->data)
                temp=temp->next;
            node->next=temp->next;
            temp->next=node;
        }
        node=save;
    }
    return head;
} 
