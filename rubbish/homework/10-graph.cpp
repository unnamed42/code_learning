#include <iostream>

#define MAX 10

struct ArcNode{
    int adjvex;
    ArcNode *nextarc;
};
template <class T> struct VNode{
    T data;
    ArcNode *firstarc;
};
template <class T> struct ALGraph{
    VNode<T> vertices[MAX];
    int vexnum,arcnum;
    int kind;
};

template <class T> void DFSArticul(ALGraph<T> &g,int v0,int visited[],int &count,int low[]){
    int min=visited[v0]=++count;
    for(auto p=g.vertices[v0].firstarc;p;p=p->nextarc){
        int w=p->adjvex;
        if(visited[w]==0){
            DFSArticul(g,w,visited,count,low);
            if(low[w]<min)
                min=low[w];
            if(low[w]>=visited[v0])
                std::cout<<g.vertices[v0].data;
        }
        else if(visited[w]<min)
            min=visited[w];
    }
    low[v0]=min;
}

template <class T> void FindArticul(ALGraph<T> &g){
    static int count=1,visited[g.arcnum]={0},low[g.arcnum]={};
    visited[0]=1;
    VNode<T> *p=g.vertices[0].firstarc;int v=p->adjvex;
    DFSArticul(g,v,visited,count,low);
    if(count<g.vexnum){
        std::cout<<g.vertices[0].data;
        while(p->nextarc){
            p=p->nextarc;v=p->adjvex;
            if(visited[v]==0)
                DFSArticul(g,v,visited,count,low);
        }
    }
} 
