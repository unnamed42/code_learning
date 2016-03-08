#ifndef __LIST__
#define __LIST__

#include <stdio.h>
#include <stdlib.h>

#ifndef NEWPTR
#   define NEWPTR(TYPE) (TYPE*)malloc(sizeof(TYPE))
#endif

#define SETUP_LIST(type,APPENDIX) \
\
typedef struct list##APPENDIX{\
    type data;\
    struct list##APPENDIX *next;\
} list##APPENDIX;\
\
list##APPENDIX *create_list##APPENDIX(const int n){\
    list##APPENDIX *head,*temp;\
    if((head=NEWPTR(list##APPENDIX))==NULL)\
        exit(EXIT_FAILURE);\
    head->data=0;\
    temp=head;\
    for(int i=1;i<n;i++){\
        if((temp->next=NEWPTR(list##APPENDIX))==NULL)\
            exit(EXIT_FAILURE);\
        temp->next->data=0;\
        temp=temp->next;\
    }\
    temp->next=NULL;\
    return head;\
}\
\
void destroy_list##APPENDIX(list##APPENDIX *head){\
    list##APPENDIX *save=NULL;\
    while(head!=NULL){\
        save=head->next;\
        free(head);\
        head=save;\
    }\
}\
\
list##APPENDIX *list_remove_elem##APPENDIX(list##APPENDIX *head,const type elem){\
    if(head->data==elem){\
        if(head->next==NULL){\
            free(head);\
            return NULL;\
        }\
        list##APPENDIX *node=head->next;\
        free(head);\
        head=NULL;\
        return list_remove_elem(node,elem);\
    }\
    list##APPENDIX *node=head->next,*prev=head;\
    while(node!=NULL){\
        if(node->data==elem){\
            prev->next=node->next;\
            free(node);\
            node=prev->next;\
        } else {\
            prev=node;\
            node=node->next;\
        }\
    }\
    return head;\
}\
\
unsigned int list_length##APPENDIX(list##APPENDIX *head){\
    unsigned int i=0;\
    for(list##APPENDIX *node=head;node!=NULL;i++,node=node->next);\
    return i;\
}\
\
int list_empty##APPENDIX(list##APPENDIX *head){\
    return head==NULL;\
}\
\
int list_search_elem##APPENDIX(list##APPENDIX *head,type elem){\
    for(int i=0;head!=NULL;i++){\
        if(head->data==elem)\
            return i;\
        head=head->next;\
    }\
    return -1;\
}\
\
list##APPENDIX *list_merge_s##APPENDIX(list##APPENDIX *head_a,list##APPENDIX *head_b){\
    if(head_a==NULL||head_b==NULL)\
        return NULL;\
    list##APPENDIX *newlist,**temp=&newlist;\
    while(head_a!=NULL&&head_b!=NULL){\
        if((*temp=NEWPTR(list##APPENDIX))==NULL)\
            exit(EXIT_FAILURE);\
        if(head_a->data<head_b->data){\
            (*temp)->data=head_b->data;\
            head_a=head_a->next;\
        } else {\
            (*temp)->data=head_a->data;\
            head_b=head_b->next;\
        }\
        temp=&(*temp)->next;\
    }\
    if(head_a==NULL){\
        while(head_b!=NULL){\
            if((*temp=NEWPTR(list##APPENDIX))==NULL)\
                exit(EXIT_FAILURE);\
            (*temp)->data=head_b->data;\
            temp=&(*temp)->next;\
            head_b=head_b->next;\
        }\
    }\
    if(head_b==NULL){\
        while(head_a!=NULL){\
            if((*temp=NEWPTR(list##APPENDIX))==NULL)\
                exit(EXIT_FAILURE);\
            (*temp)->data=head_a->data;\
            temp=&(*temp)->next;\
            head_a=head_a->next;\
        }\
    }\
    *temp=NULL;\
    return newlist;\
}\
\
void list_insert_elem##APPENDIX(list##APPENDIX *head,int index,type elem){\
    for(int i=0;i<index;i++){\
        if(head==NULL)\
            return;\
        head=head->next;\
    }\
    list##APPENDIX *node;\
    if((node=NEWPTR(list##APPENDIX))==NULL)\
        exit(EXIT_FAILURE);\
    node->data=elem;\
    node->next=head->next;\
    head->next=node;    \
}\
\
void list_remove_after##APPENDIX(list##APPENDIX *head,int index){\
    for(int i=0;i<index;i++){\
        if(head==NULL)\
            return;\
        head=head->next;\
    }\
    if(head->next==NULL)\
        return;\
    list##APPENDIX *save=head->next->next;\
    free(head->next);\
    head->next=save;\
}\
\
list##APPENDIX *list_reverse##APPENDIX(list##APPENDIX *head){\
    if(head==NULL||head->next==NULL)\
        return head;\
    list##APPENDIX *nhead=NULL;\
    while(head!=NULL){\
        list##APPENDIX *node=head->next;\
        head->next=nhead;\
        nhead=head;\
        head=node;\
    }\
    return nhead;\
}\
\
void list_input_data##APPENDIX(list##APPENDIX *head){\
    while(head!=NULL){\
        scanf("%d",&head->data);\
        head=head->next;\
    }\
}\
\
void print_list##APPENDIX(list##APPENDIX *head){\
    while(head!=NULL){\
        printf("node %p, data %d, next one %p\n",head,head->data,head->next);\
        head=head->next;\
    }\
}

#endif