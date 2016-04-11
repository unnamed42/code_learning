#include "all.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <string>
#include <random>

using namespace rubbish;

struct test{
    int data;
    test(int n=0):data(n){}
    void increment(){data++;}
};

int val(helper::avl_tree_node<int> *p) {return p==nullptr?-1:p->data;}

int main(){
    forward_list_base<test> l;
    for(int i=0;i<10;i++)
        l.push_back(test(i));
    for(auto &i:l)
        i.increment();
    std::for_each(l.begin(),l.end(),[](test &t){std::cout<<t.data<<" ";});
    
    list_base<int> list;
    // Random integer generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1,100);
    for(auto i=0;i<10;++i)
        list.push_back(dist(gen));
    for(auto &i:list)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    list.sort();
    for(auto &i:list)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    for(auto it=list.rbegin();it!=list.rend();++it)
        std::cout<<*it<<" ";
    
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
    
    avl_tree<int> te({1,2,3,3,3,86,651,32,41,651,984,16,19,41,0});
    for(auto it=te.level_begin();it!=te.level_end();++it){
        auto &&ptr=it.get();
        std::cout<<"this:"<<val(ptr)<<" left:"<<val(ptr->left)<<" right:"<<val(ptr->right)<<" parent:"<<val(ptr->parent)<<std::endl;
    }
    for(auto &i:te)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    te.erase(3);
    for(auto &i:te)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    auto x=te.insert(195);
    std::cout<<(x!=te.end()?*x:0)<<std::endl;
    for(auto &i:te)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    
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
    
    continuous_container<int> cc(10,2);
    for(auto &i:cc)
        std::cout<<i<<" ";
    
    const std::string str("abc");
    map<std::string,int> m={{"abc",2568},{"ffg",45},{"eat",445}};
    std::cout<<m.find(std::string("abc"))->second<<" ";
    m.find(std::string(str))->second+=554;
    std::cout<<m.find(std::string(str))->second;
    m.erase(std::string("ffg"));
    for(auto &i:m)
        std::cout<<i.first<<" "<<i.second;
    
    return 0;
}
