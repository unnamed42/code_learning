#include <stdio.h>
#include <stdlib.h>

#ifndef __ARRAY__
#define __ARRAY__

#define SETUP_ARRAY(type,APPENDIX)\
\
typedef struct{\
    type *base;\
    int length;\
    int occupied;\
}array##APPENDIX;\
\
void init_array##APPENDIX(array##APPENDIX *arr){\
    arr->base=NULL;\
    arr->length=0;\
    arr->occupied=0;\
}\
\
void create_array##APPENDIX(array##APPENDIX *arr,int n){\
    printf("Input %d data:",n);\
    arr->base=(type*)malloc(sizeof(type)*(n+1));\
    for(int i=0;i<n;i++)\
        scanf("%d",arr->base+i);\
    arr->length=n+1;\
    arr->occupied=n;\
}\
\
void destroy_array##APPENDIX(array##APPENDIX *arr){\
    free(arr->base);\
    free(arr);\
}\
\
void print_array##APPENDIX(array##APPENDIX *arr){\
    printf("%d data in length %d\n",arr->occupied,arr->length);\
    for(int i=0;i<arr->occupied;i++)\
        printf("%d ",*(arr->base+i));\
    printf("\n");\
}\
\
void resize_array##APPENDIX(array##APPENDIX *arr,int size){\
    void *newptr=realloc(arr->base,sizeof(type)*size);\
    if(newptr==NULL)\
        return;\
    arr->base=(type*)newptr;\
    arr->length=size;\
}\
\
void array_append##APPENDIX(array##APPENDIX *arr,type elem){\
    if(arr->occupied+1>arr->length)\
        resize_array##APPENDIX(arr,2*arr->length);\
    *(arr->base+arr->occupied-1)=elem;\
    arr->occupied++;\
}\
\
int array_locate##APPENDIX(array##APPENDIX *arr,int elem,type (*compare)(type,type)){\
    for(int i=0;i<arr->occupied;i++){\
        if(compare(*(arr->base+i),elem))\
            return i;\
    }\
    return -1;\
}\
\
int array_prior_elem##APPENDIX(array##APPENDIX *arr,type elem){\
    for(int i=0;i<arr->occupied;i++){\
        if(i!=0&&*(arr->base+i)==elem)\
            return i-1;\
    }\
    return -1;\
}\
\
int array_next_elem(array##APPENDIX *arr,type elem){\
    for(int i=arr->occupied-1;i>=0;i++){\
        if(i!=arr->occupied-1&&*(arr->base+i)==elem)\
            return i+1;\
    }\
    return -1;\
}\
\
void array_insert##APPENDIX(array##APPENDIX *arr,int index,type elem){\
    if(index>arr->occupied)\
        return;\
    if(arr->occupied+1>arr->length)\
        resize(arr,2*arr->length);\
    if(index==arr->occupied){\
        *(arr->base+index)=elem;\
        arr->occupied++;\
        return;\
    }\
    for(int i=arr->occupied;i>index;i--)\
        *(arr->base+i)^=*(arr->base+i-1)^=*(arr->base+i)^=*(arr->base+i-1);\
    *(arr->base+index)=elem;\
    arr->occupied++;\
}\
\
void array_remove_elem##APPENDIX(array##APPENDIX *arr,type elem){\
    for(int i=0;i<arr->occupied;i++){\
        if(*(arr->base+i)==elem){\
            for(int j=i;j<arr->occupied-1;j++)\
                *(arr->base+j)^=*(arr->base+j+1)^=*(arr->base+j)^=*(arr->base+j+1);\
            arr->occupied--;\
            i--;\
        }\
    }\
}

#endif