/* 以下修改建议以C99为标准 */

#include <stdio.h>
#include <stdlib.h>

#define ERROR 0

/* C99引入了布尔类型，出于某种我不知道的原因它的关键字是_Bool
 * 为了使用上的方便，可以考虑包含头文件stdbool.h, 里面有bool, true, false的宏定义
 * 另外，宏定义末尾一般不应该有分号
 * 原代码:
 * # define TRUE 1;
 */
#include <stdbool.h>

/* 这是我用的一个小技巧，可以有效避免见到malloc那个家伙 */
#define NEW(type) (type*)malloc(sizeof(type))

/* C99中有没有“前向声明”这个概念我不清楚
 * 但就算是在C++中前向声明技巧是用来解决难以解决的类之间的循环依赖问题
 * 你确定你真的需要这个东西？
 * typedef struct Passenger Passenger;
 * typedef struct Queue Queue;
 */

/* 乘客的结构体并不需要组成链表
 * 链式队列的节点结构体才需要链表
 * 结构体部分略作修改后的原代码:
typedef struct Passenger{
    int time;
    int id;
    int bus;
    struct Passenger* next;
} Passenger;
 
typedef struct{
    Passenger* front;
    Passenger* rear;
    int length;
}Queue;
*/

/* 甚至说我们可以连Passenger都不要，因为上车时乘客想上的车号在函数里就可以判断
 * 需要保留的就只有到达时间
 */
typedef struct QNode{
    int data;           //到达时间
    struct QNode *next;
}QNode;

typedef struct{
    QNode* front;
    QNode* rear;
    int length;
}Queue;

/* 全局变量，好暴力
 * 原代码:
Queue* Q;
Queue* Q1,*Q2,*Q3,*Q4;*/

/* 要么加返回值，要么使用二重指针
 * 原代码:
void InitQueue(Queue *Q){
    Q->front=Q->rear=(Passenger*)malloc(sizeof(Passenger));
    Q->front->next=NULL;
}
 */
Queue* InitQueue(){
    /* 在队列初始化的时候，一定要先给队列本身的结构体分配空间，此后记得free */
    Queue *Q=NEW(Queue);
    
    Q->front=Q->rear=NEW(QNode);
    Q->front->next=NULL;
    
    /* 自己写的length写着就不管了 */
    Q->length=0;
    return Q;
}

void EnQueue(Queue *Q,int time){
    Q->rear->next=NEW(QNode);
    Q->rear->next->data=time;
    Q->rear->next->next=NULL;
    Q->rear=Q->rear->next;
    
    Q->length++;
}

/* 神一般的函数
 * 首先，void类型决定了它不应该有返回值
 * 另外，if里可以有连等号，但是其结果就不是你想象的那样了
 * 原代码:
void DeQueue(Queue* Q,Passenger* e){
    if(Q->front==Q->rear==NULL)return ERROR;
    e=Q->front;
    Q->front=e->next;return e;
}
 */
int DeQueue(Queue* Q){
    if(Q->length==0){
        perror("Queue empty");  //作用同puts，但puts是向stdout里输出，perror是向stderr输出
        return -1;
    }
    QNode *e=Q->front;
    Q->front=e->next;
    int result=e->data;
    free(e);
    Q->length--;
    return result;
}

/* 添加的删除函数 */
void Destroy(Queue *Q){
    QNode *node=Q->front;
    while(node!=NULL){
        QNode *save=node->next;
        free(node);
        node=save;
    }
    free(Q);
}

/* id我给去掉了
 * 由于到达时间是随机数生成的，所以大小并不好确定，因此需要使用数组存放并在随机完后排序
 * 而使用随机数生成两名乘客的到达时间间隔就不用排序，可保证到达时间是增序的
 * 因此在此假定，到达时间间隔不超过5min
 * 原代码:
void CreatePassenger(Passenger *a){
    int i;int j;int t;
    for(i=0;i<8000;i++){
        a[i].id=i;
        a[i].bus=rand()%4+1;
        a[i].time=rand()%961;
    }
    for(i=0;i<8000-1;i++){
        for(j=0;j<8000-i-1;j++){
            if(a[j].time>a[j+1].time){
                t=a[j].time;
                a[j].time=a[j+1].time;
                a[j+1].time=t;
                printf("%d%d",a[j].time,a[j+1].time);
            }
        }
    }
}

void Getonbus(Passenger a[8000]){
    int i;Q->length=0;
    for(i=0;i<8000-1;i++){
        switch(a[i].bus){
            
            //下面是使用switch case时最容易犯的错误之一，就是末尾没有break
            
            case 1:EnQueue(Q1,a+i);Q1->length++;break;
            case 2:EnQueue(Q2,a+i);Q2->length++;break;
            case 3:EnQueue(Q3,a+i);Q3->length++;break;
            case 4:EnQueue(Q4,a+i);Q4->length++;break;
        }
    }
}

double Caltime(Queue *Q,int interval,int start){
    int i;int k;int average;
    int nums=960/k;Passenger* e;int sum=0;
    for(k=0;k<nums;k++){
        for(i=0;i<Q->length;i++){
            e=Q->front;
            if(e->time>((k-1)*interval+start)&&e->time<(k*interval+start)){
                sum=sum+(k*interval+start)-(e->time);
                e=e->next;
            }
        }
    }
    return sum;
}
*/

/* 上面去掉了全局变量的方式因此这里需要传入一个队列指针数组Queue*[]
 * 每个队列代表不同的车
 * 另外，使用MAX代替8000
 */
#define MAX 8000
void GetonBus(Queue *bus[]){
    int arrival=0;
    for(int i=0;i<MAX;i++){
        EnQueue(bus[rand()%4],arrival); //rand()%4正好代表了bus数组里对应的下标
        arrival+=rand()%6;
    }
}

double Caltime(Queue *Q,int interval,int start){
    long long sum=0;int length=Q->length,time=0;
    for(int i=0;i<length;i++){
        time=DeQueue(Q);
        while(time>start)
            start+=interval;
        sum+=start-time;
    }
    return (double)sum/length;
}

int main(){
    
    /* 堪称劳模
    int interval1,interval2,interval3,interval4;
    int start1,start2,start3,start4;double time1,time2,time3,time4;
    scanf("%d",&interval1);
    scanf("%d",&interval2);
    scanf("%d",&interval3);
    scanf("%d",&interval4);
    scanf("%d",&start1);
    scanf("%d",&start2);
    scanf("%d",&start3);
    scanf("%d",&start4);
    Passenger b[8000];
    CreatePassenger(b);
    Getonbus(b);
    time1=Caltime(Q1,interval1,start1);
    time2=Caltime(Q2,interval2,start2);
    time3=Caltime(Q3,interval3,start3);
    time4=Caltime(Q4,interval4,start4);
    //printf("%d",time1);
    //printf("%d",time1);
    //printf("%d",time1);
    //printf("%d",time1);
    */
    Queue *bus[4];
    for(int i=0;i<4;i++)
        bus[i]=InitQueue();
    GetonBus(bus);
    for(int i=0;i<4;i++){
        int start,interval;
        printf("Input start time of bus %c:",'A'+i);
        scanf("%d",&start);
        printf("Input interval of bus %c:",'A'+i);
        scanf("%d",&interval);
        double time=Caltime(bus[i],interval,start);
        printf("\nAverage time of bus %c is:%lf\n",'A'+i,time);
    }
    for(int i=0;i<4;i++)
        Destroy(bus[i]);
    return 0;
}