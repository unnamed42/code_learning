#include <stdio.h>
#include <stdlib.h>

#ifndef __STACK__
#define __STACK__

#ifndef NEWPTR_L
#   define NEWPTR_L(type,length) (type*)malloc((length)*sizeof(type))
#endif

#ifndef NEWPTR
#   define NEWPTR(type) (type*)malloc(sizeof(type))
#endif

#define SETUP_STACK(type,APPENDIX)\
\
typedef struct{\
    type *base;\
    type *top;\
    unsigned int size;\
} stack##APPENDIX;\
\
stack##APPENDIX *init_stack##APPENDIX(){\
    stack##APPENDIX *s=NEWPTR(stack##APPENDIX);\
    s->base=s->top=NULL;\
    s->size=0;\
    return s;\
}\
\
stack##APPENDIX *create_stack##APPENDIX(int sz){\
    stack##APPENDIX *s=NEWPTR(stack##APPENDIX);\
    s->top=s->base=NEWPTR_L(type,sz);\
    s->size=sz;\
    return s;\
}\
\
void destroy_stack##APPENDIX(stack##APPENDIX *s){\
    free(s->base);\
    free(s);\
}\
\
unsigned int stack_length##APPENDIX(stack##APPENDIX *s){\
    return (s->top-s->base);\
}\
\
void resize_stack##APPENDIX(stack##APPENDIX *s,int sz){\
    type *temp=(type*)realloc(s->base,sz*sizeof(type));\
    if(temp==NULL){\
        perror("Bad alloc");\
        exit(EXIT_FAILURE);\
    }\
    s->base=temp;\
    s->size=sz;\
}\
\
void stack_push##APPENDIX(stack##APPENDIX *s,type data){\
    if((s->top-s->base)>=s->size)\
        resize_stack##APPENDIX(s,2*s->size);\
    *(s->top)=data;\
    s->top++;\
}\
\
type stack_pop##APPENDIX(stack##APPENDIX *s){\
    if(s->top==s->base){\
        perror("Stack empty!\n");\
        exit(EXIT_FAILURE);\
    }\
    s->size--;\
    return *(--s->top);\
}\
\
void print_stack##APPENDIX(stack##APPENDIX *s){\
    type *temp=s->base;\
    while(temp!=s->top){\
        printf("%p %d\n",temp,*temp);\
        temp++;\
    }\
}\
\
type stack_top##APPENDIX(stack##APPENDIX *s){\
    if(s->top==s->base){\
        perror("Stack empty!\n");\
        exit(EXIT_FAILURE);\
    }\
    return *(s->top-1);\
}

#endif