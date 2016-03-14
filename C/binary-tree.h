#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include <stdio.h>
#include <stdlib.h>
#include "linked-queue.h"

#ifndef NEWPTR
#   define NEWPTR(type) ((type)*)malloc(sizeof((type)))
#endif

typedef enum {LEFT=0,RIGHT} CHILD;

#define SETUP_BINARY_TREE(type,APPENDIX) \
\
typedef struct bi_tree##APPENDIX{\
    type data;\
    struct bi_tree##APPENDIX *left,*right;\
}bi_tree##APPENDIX;\
\
SETUP_QUEUE(bi_tree##APPENDIX*,APPENDIX)\
\
static bi_tree##APPENDIX *new_tree##APPENDIX(type data){\
    bi_tree##APPENDIX *p=NEWPTR(bi_tree##APPENDIX);\
    p->data=data;\
    p->left=p->right=NULL;\
    return p;\
}\
\
static void destroy_tree##APPENDIX(bi_tree##APPENDIX *root){\
    if(root==NULL)\
        return;\
    destroy_tree##APPENDIX(root->left);\
    destroy_tree##APPENDIX(root->right);\
    free(root);\
}\
\
static unsigned int depth##APPENDIX(bi_tree##APPENDIX *root){\
    if(root==NULL)\
        return 0;\
    unsigned int left=depth##APPENDIX(root->left),right=depth##APPENDIX(root->right);\
    return 1+(left>right?left:right);\
}\
\
static void insert_parent##APPENDIX(bi_tree##APPENDIX **root,type data,CHILD child){\
    bi_tree##APPENDIX *p=new_tree##APPENDIX(data);\
    switch(child){\
        case LEFT:p->left=*root;break;\
        case RIGHT:p->right=*root;break;\
    }\
    *root=p;\
}\
\
static void insert_child##APPENDIX(bi_tree##APPENDIX *root,type data,CHILD child){\
    bi_tree##APPENDIX *p=new_tree##APPENDIX(data);\
    switch(child){\
        case LEFT:\
            if(root->left!=NULL)\
                destroy_tree##APPENDIX(root->left);\
            root->left=p;\
            break;\
        case RIGHT:\
            if(root->right!=NULL)\
                destroy_tree##APPENDIX(root->right);\
            root->right=p;\
            break;\
    }\
}\
\
static void pre_traverse##APPENDIX(bi_tree##APPENDIX *root,void (*func)(type)){\
    if(root==NULL)\
        return;\
    func(root->data);\
    pre_traverse##APPENDIX(root->left,func);\
    pre_traverse##APPENDIX(root->right,func);\
}\
\
static void in_traverse##APPENDIX(bi_tree##APPENDIX *root,void (*func)(type)){\
    if(root==NULL)\
        return;\
    in_traverse##APPENDIX(root->left,func);\
    func(root->data);\
    in_traverse##APPENDIX(root->right,func);\
}\
\
static void post_traverse##APPENDIX(bi_tree##APPENDIX *root,void (*func)(type)){\
    if(root==NULL)\
        return;\
    post_traverse##APPENDIX(root->left,func);\
    post_traverse##APPENDIX(root->right,func);\
    func(root->data);\
}\
\
static void level_traverse##APPENDIX(bi_tree##APPENDIX *root,void (*func)(type)){\
    queue##APPENDIX *q=init_queue##APPENDIX();\
    queue_push##APPENDIX(q,root);\
    while(!queue_empty##APPENDIX(q)){\
        bi_tree##APPENDIX *p=queue_pop##APPENDIX(q);\
        func(p->data);\
        if(p->left!=NULL)\
            queue_push##APPENDIX(q,p->left);\
        if(p->right!=NULL)\
            queue_push##APPENDIX(q,p->right);\
    }\
    destroy_queue##APPENDIX(q);\
}

#endif