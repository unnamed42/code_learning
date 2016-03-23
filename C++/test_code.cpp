#include "all.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <initializer_list>

using namespace rubbish;

struct test{
    int data;
    test(int n=0):data(n){}
    void increment(){data++;}
};

int main(){
    forward_list_base<test> l;
    for(int i=0;i<10;i++)
        l.push_back(test(i));
    for(auto &i:l)
        i.increment();
    std::for_each(l.begin(),l.end(),[](test &t){std::cout<<t.data<<" ";});
    
    graph<int> a({0,1,2,3,4,5,6},{{0,1},{0,2},{1,3},{2,3},{2,4},{3,5},{4,5}},true);
    std::vector<int> sv;
    struct op{
        std::vector<int> *p;
        void operator()(int c){
            if(p==nullptr)
                std::cout<<c;
            else
                p->push_back(c);
        }
        explicit op(std::vector<int> *_p=nullptr):p(_p){}
    };
    for(auto &i:a)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    for(auto i=a.bfs_begin();i!=a.bfs_end();++i)
        std::cout<<*i<<" ";
    std::cout<<std::endl;
    for(auto i=a.topo_begin();i!=a.topo_end();++i)
        std::cout<<*i<<" ";
    std::cout<<"\nEnd";
    
    vector<int> v{0,1,2,3,45};
    for(auto &i:v)
        std::cout<<i<<" ";
    
    std::initializer_list<int> &&p={1,1,1,2,56,9,44,100,9999999};
    avl_tree<int> tree({1,1,1,2,56,9,44,100,9999999});
    for(auto &i:tree)
        std::cout<<i<<" ";
    for(auto &&i:p){
        tree.erase(i);
        for(auto &j:tree)
            std::cout<<j<<" ";
        std::cout<<std::endl;
    }
    
    bsearch_tree<int> treea({1,5,7,9,5,25,0,1,9,4,1});
    for(auto &i:treea)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    for(auto it=treea.level_begin();it!=treea.level_end();++it)
        std::cout<<(*it)<<" ";
    std::cout<<"\nExit successfully.";
    
    binary_tree_base<int> treeb({1,2,3,4,5,6,7,8,9,10},0);
    for(auto it=treeb.preorder_begin();it!=treeb.preorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=treeb.inorder_begin();it!=treeb.inorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=treeb.postorder_begin();it!=treeb.postorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=treeb.level_begin();it!=treeb.level_end();++it)
        std::cout<<(*it)<<",";
    binary_tree_base<int> treeba({1,2,4,8,9,5,10,3,6,7},{8,4,9,2,10,5,1,6,3,7});
    for(auto it=treeba.level_begin();it!=treeba.level_end();++it)
        std::cout<<(*it)<<",";
    
    continuous_container<int> x(10,2);
    for(auto &i:x)
        std::cout<<i<<" ";
    
    return 0;
}
