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

// Insertion sort method
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

// Merge sort method

// merge_sort helper
void merge(list *h1,list *h2){
    list dummy;list *ptr=&dummy;
    while(h1!=NULL&&h2!=NULL){
        if(h1->data<h2->data){
            ptr->next=h1;
            h1=h1->next;
        } else {
            ptr->next=h2;
            h2=h2->next;
        }
        ptr=ptr->next;
    }
    if(h1!=NULL)
        ptr->next=h1;
    if(h2!=NULL)
        ptr->next=h2;
    return dummy.next;
}

void merge_sort(list *head){
    if(head==NULL||head->next==NULL)
        return head;
    if(head->next->next==NULL){
        list *node=head->next;
        if(node->data<head->data){
            node->next=head;
            head->next=NULL;
            head=node;
        }
        return head;
    }
    list *slow=head,*fast=head->next->next;
    while(fast!=NULL){
        slow=slow->next;
        fast=fast->next;
        if(fast!=NULL)
            fast=fast->next;
    }
    // Reuse fast
    fast=merge_sort(slow->next);
    slow->next=NULL;
    return merge(merge_sort(head),fast);
}

