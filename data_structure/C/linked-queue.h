#ifndef __LINKED_QUEUE__
#define __LINKED_QUEUE__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef NEWPTR
#   define NEWPTR(type) (type*)malloc(sizeof(type))
#endif

#define SETUP_QUEUE(type,APPENDIX) \
\
typedef struct qnode##APPENDIX{\
    type data;\
    struct qnode##APPENDIX *next;\
}qnode##APPENDIX;\
\
typedef struct{\
    qnode##APPENDIX *front,*rear;\
    unsigned int length;\
}queue##APPENDIX;\
\
static qnode##APPENDIX *new_qnode##APPENDIX(type data){\
    qnode##APPENDIX *node=NEWPTR(qnode##APPENDIX);\
    node->data=data;\
    node->next=NULL;\
    return node;\
}\
\
static queue##APPENDIX *init_queue##APPENDIX(){\
    queue##APPENDIX *q=NEWPTR(queue##APPENDIX);\
    q->front=q->rear=NULL;\
    q->length=0;\
    return q;\
}\
\
static void destroy_queue##APPENDIX(queue##APPENDIX *q){\
    while(q->front!=NULL){\
        q->rear=q->front->next;\
        free(q->front);\
        q->front=q->rear;\
    }\
    free(q);\
}\
\
static void queue_push##APPENDIX(queue##APPENDIX *q,type data){\
    if(q->rear==NULL){\
        q->rear=q->front=new_qnode##APPENDIX(data);\
        q->length=1;\
        return;\
    }\
    q->rear->next=new_qnode##APPENDIX(data);\
    q->rear=q->rear->next;\
    q->length++;\
}\
\
static type queue_front##APPENDIX(queue##APPENDIX *q){\
    if(q->front==NULL){\
        perror("Queue empty");\
        exit(EXIT_FAILURE);\
    }\
    return q->front->data;\
}\
\
static type queue_pop##APPENDIX(queue##APPENDIX *q){\
    if(q->front==NULL){\
        perror("Queue empty");\
        exit(EXIT_FAILURE);\
    }\
    type data=q->front->data;\
    if(q->front->next==NULL){\
        free(q->front);\
        q->front=q->rear=NULL;\
        return data;\
    }\
    qnode##APPENDIX *save=q->front->next;\
    free(q->front);\
    q->front=save;\
    q->length--;\
    return data;\
}\
\
static unsigned int queue_length##APPENDIX(queue##APPENDIX *q){return q->length;}\
\
static bool queue_empty##APPENDIX(queue##APPENDIX *q){return q->front==NULL;}

#endif
