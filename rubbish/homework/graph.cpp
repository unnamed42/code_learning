/* 以下修改建议以C++11为标准 */

#include <iostream>
#include <stack>

/* using namespace std最好不要用，这是一个习惯问题，现在程序小可能问题不大
 * 如果实在是想偷懒的话用using std::cin; 之类的，cin就可以不用写成std::cin这样了(但其他的还是要std::)
 */
using namespace std;

#define MAX 10

/* 这类提前起别名来达到范式编程的手段，效果不好
 * 万一我在一个程序里面VertexType既有int又有char怎么办？
 * 可以考虑使用C++的模板特性
 */
typedef int InfoType;
typedef int VertexType;

/* 关于空指针的问题：
 * 在C++11中，专门引入了新的关键字nullptr来代替原来老式的NULL
 * 在C++中，处于兼容C的考虑保留了NULL，但出于各种原因不能允许void*的隐式类型转换
 * 因此NULL在C++中是int类型的 0 而不是和C语言一样是(void*)0
 * 而且int *p=nullptr;delete p;这样的语句不会出现问题，不用像free一样特地判断是否为空
 */

/* 关于new的问题：
 * new比malloc多了一项任务，就是在开辟空间成功后调用构造函数初始化
 * 因此如果你提供了空构造函数，那么用new的时候就能方便很多
 * 其次，new在分配失败的时候会抛出异常std::bad_alloc, 因此不用像malloc分配方式一样
 * 特地去检测分配得来的指针是否为空
 */

struct ArcNode{
    int adjvex;
    ArcNode *nextarc;
    InfoType *info;
};


/* 建议添加VNode的空构造函数 */
struct VNode{
    VertexType data;
    ArcNode *firstarc;
    
};

/* 同上，构造函数 */
struct ALGraph{
    ArcNode** G;
    int vexnum,arcnum;
    int kind;
};

void InsertList(ArcNode* a,ArcNode* b){
    ArcNode* node=new ArcNode;
    node->adjvex=b->adjvex;
    node->info=b->info;
    a->nextarc=node;
}

/* 要是用C++的话，请改造成构造函数 */
ArcNode* List(VNode* v){
    ArcNode* head=new ArcNode;
    head->adjvex=v->data;
    v->firstarc=head;
    int vnum;cin >>vnum;
    int i;
    ArcNode* p=new ArcNode;
    ArcNode* q=new ArcNode;
    p=head; 
    for(i=0;i<vnum;i++){
        cin >>q->adjvex;
        InsertList(p,q);
        p=p->nextarc ;
    }
    p->nextarc=NULL;
    return head;
}

void Array(ALGraph* g){
    /* 像这样和类紧密相关的操作，建议改造成类的成员函数
     * 也不会出现这里两个G这么混乱的事情
     */
    ArcNode **G;
    G=g->G=new ArcNode*[MAX];
    /* 你也真是不嫌累…… 
     * 原代码:
    VNode* v1=new VNode;cin >>v1->data;
    VNode* v2=new VNode;cin >>v2->data;
    VNode* v3=new VNode;cin >>v3->data;
    VNode* v4=new VNode;cin >>v4->data;
    VNode* v5=new VNode;cin >>v5->data;
    VNode* v6=new VNode;cin >>v6->data;
    VNode* v7=new VNode;cin >>v7->data;
    VNode* v8=new VNode;cin >>v8->data;
    VNode* v9=new VNode;cin >>v9->data;
    VNode* v10=new VNode;cin >>v10->data;
    G[1]=List(v1);
    G[2]=List(v2);
    G[3]=List(v3);
    G[4]=List(v4);
    G[5]=List(v5);
    G[6]=List(v6);
    G[7]=List(v7);
    G[8]=List(v8);
    G[9]=List(v9);
    G[10]=List(v10);*/
    for(int i=0;i<MAX;i++){
       VNode *v=new VNode;
       std::cin>>v->data;
       G[i]=List(v);
    }
}

int main(){
    ALGraph* graph=new ALGraph;
    Array(graph);
    return 0;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    