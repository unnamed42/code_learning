#include <stdio.h>
float pscore(int* p){
    int *max,*min,i;float s=0;
    max=min=p;
    for(i=0;i<10;i++){
        if(*max<*(p+i))
            max=(p+i);
        if(*min>*(p+i))
            min=(p+i);
    }
    *min=0;*max=0;
    for(i=0;i<10;i++)
        s+=*(p+i);
    return s/8.0;
}

int main(){
    int score[10],i;float final_score;
    printf("Please input scores:\n");
    for(i=0;i<10;i++)
        scanf("%d",&score[i]);
    final_score=pscore(score);
    printf("final score is %f\n",final_score);
    return 0;
}