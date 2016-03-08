#ifndef __DOUBLE_LIST__
#define __DOUBLE_LIST__

#include <stdio.h>
#include <stdlib.h>

#ifndef NEWPTR
#   define NEWPTR(type) (type*)malloc(sizeof(type))
#endif

#define SETUP_LIST(type,APPENDIX) \
\
typedef struct list##APPENDIX{\
    type data;\
    struct list##APPENDIX *prev;\
    struct list##APPENDIX *next;\
}list##APPENDIX;\
\
list##APPENDIX *create_list##APPENDIX(int n){\
    list##APPENDIX *head,*temp;\
    head=NEWPTR(list##APPENDIX);\
    head->prev=NULL;\
    temp=head;\
    for(;n>0;n--){\
        temp->next=NEWPTR(list##APPENDIX);\
        temp->next->prev=temp;\
        temp=temp->next;\
    }\
    temp->next=NULL;\
    return head;\
}\
\
void destroy_list##APPENDIX(list##APPENDIX *head){\
    list##APPENDIX *save;\
    while(head!=NULL){\
        save=head->next;\
        free(head);\
        head=save;\
    }\
}\
\
list##APPENDIX *list_remove_elem##APPENDIX(list##APPENDIX *head,type elem){\
    if(head->data==elem){\
        if(head->next==NULL){\
            free(head);\
            return NULL;\
        }\
        list##APPENDIX *node=head->next;\
        free(head);\
        return list_remove_elem(node,elem);\
    }\
    list##APPENDIX *node=head->next;\
    while(node!=NULL){\
        if(node->data==elem){\
            list##APPENDIX *save=node->next;\
            node->prev->next=node->next;\
            node->next->prev=node->prev;\
            free(node);\
            node=save;\
        } else\
            node=node->next;\
    }\
    return head;\
}\
\
unsigned int list_length##APPENDIX(list##APPENDIX *head){\
    unsigned int i=0;\
    for(;head!=NULL;i++,head=head->next);\
    return i;\
}\
\
list##APPENDIX *list_insert_elem##APPENDIX(list##APPENDIX *head,int index,type elem){\
    list##APPENDIX *save=head;\
    if(index==0){\
        list##APPENDIX *_head=NEWPTR(list##APPENDIX);\
        _head->prev=NULL;_head->next=head;\
        _head->data=elem;\
        return _head;\
    } else {\
        for(int i=0;i<index;i++){\
            if(head==NULL)\
                return save;\
            head=head->next;\
        }\
        list##APPENDIX *temp=NEWPTR(list##APPENDIX);\
        temp->data=elem;\
        temp->next=head;\
        temp->prev=head->prev;\
        head->prev=temp;\
        return head;\
    }\
}\
\
list##APPENDIX *list_reverse##APPENDIX(list##APPENDIX *head){\
    if(head==NULL||head->next==NULL)\
        return head;\
    while(head->next!=NULL){\
        list##APPENDIX *temp=head->prev;\
        head->prev=head->next;\
        head->next=temp;\
        head=head->prev;\
    }\
    return head;\
}\
\
void list_input##APPENDIX(list##APPENDIX *head){\
    while(head!=NULL){\
        scanf("%d",&head->data);\
        head=head->next;\
    }\
}\
\
void print_list##APPENDIX(list##APPENDIX *head){\
    while(head!=NULL){\
        printf("previous one %p,node %p, data %d, next one %p\n",head->prev,head,head->data,head->next);\
        head=head->next;\
    }\
}\

#endif