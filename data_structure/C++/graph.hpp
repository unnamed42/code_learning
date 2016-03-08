#ifndef __GRAPH__
#define __GRAPH__

#include <deque>
#include <vector>
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include <stdexcept>
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

template <class T> class graph{
    public:
        typedef int iterator_index_t;
        typedef unsigned int index_t;
        
        template <class U> struct node{
            U data;
            node<index_t> *next;
            explicit node();
            explicit node(const U&);
            explicit node(const node<U>*);
        };
        
        typedef node<T> vex_node;
        typedef node<index_t> arc_node;
        
        class iterator_base{
            public:
                typedef iterator_base self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                typedef vex_node* real_pointer;
                explicit iterator_base(iterator_index_t _index,std::vector<real_pointer>* =nullptr);
                reference operator*() const;
                pointer operator->() const;
                iterator_index_t get() const;
                bool operator==(const self_type &other) const;
                bool operator!=(const self_type &other) const;
            protected:
                iterator_index_t index;
                std::vector<real_pointer> *nodes;
        };
        
        class dfs_iterator: public iterator_base{
            public:
                typedef dfs_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit dfs_iterator(iterator_index_t _index,std::vector<real_pointer>* =nullptr,std::deque<iterator_index_t>* =nullptr);
                self_type& operator++();
                self_type operator++(int);
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::index;
                using iterator_base::nodes;
                iterator_index_t component; // Set `component` to iterate over isolated nodes.
                std::shared_ptr<bool> visited;
                std::shared_ptr<std::deque<iterator_index_t>> s;//stack
        };
        
        class bfs_iterator: public iterator_base{
            public:
                typedef bfs_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit bfs_iterator(iterator_index_t _index,std::vector<real_pointer>* =nullptr,std::deque<iterator_index_t>* =nullptr);
                self_type& operator++();
                self_type operator++(int);
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::index;
                using iterator_base::nodes;
                iterator_index_t component;
                std::shared_ptr<bool> visited;
                std::shared_ptr<std::deque<iterator_index_t>> q;//queue
        };
        
        class topo_iterator: public iterator_base{
            public:
                typedef topo_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit topo_iterator(iterator_index_t _index,std::vector<real_pointer>* =nullptr,unsigned int count=0,int stack=-1,int *indegree=nullptr);
                self_type& operator++();
                self_type operator++(int);
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::index;
                using iterator_base::nodes;
                int stack;
                unsigned int count;
                std::shared_ptr<int> indegree;
        };
        
        typedef dfs_iterator iterator;
        
    protected:
        std::vector<vex_node*> nodes;
        bool directed;
    public:
        explicit graph(bool=false);
        graph(const graph<T> &g);
        graph(std::vector<T>&&,std::vector<std::pair<index_t,index_t>>&&,bool=false);
        ~graph();
        void append(const T &data);
        void set_directed(bool is=true);
        bool empty() const;
        bool is_directed() const;
        void bind(index_t vex1,index_t vex2);
        T& operator[](index_t i);
        
        dfs_iterator begin();
        dfs_iterator end();
        bfs_iterator bfs_begin();
        bfs_iterator bfs_end();
        topo_iterator topo_begin();
        topo_iterator topo_end();
};

// Member functions
template <class T> graph<T>::graph(bool dir):nodes(),directed(dir){}

template <class T> graph<T>::graph(const graph<T> &g){
    iterator_index_t size=g.nodes.size();
    for(iterator_index_t i=0;i<size;i++){
        nodes.push_back(new vex_node(g.nodes[i]));
        arc_node **ptr=&nodes[i].next,*ptr_g=g.nodes[i].next;
        while(ptr_g!=nullptr){
            *ptr=new arc_node(ptr_g->data);
            ptr=&(*ptr)->next;ptr_g=ptr_g->next;
        }
    }
    directed=g.directed;
}

// construct the adjacent list from value list and adjacency list, do not check duplicate in adjacency.
template <class T> graph<T>::graph(std::vector<T> &&value,std::vector<std::pair<index_t,index_t>> &&adjacency,bool _directed):directed(_directed){
    auto size=value.size();
    if(size==0)
        return;
    for(auto &&i:value)
        nodes.push_back(new vex_node(i));
    for(auto &&i:adjacency){
        if(i.first>=size||i.second>=size)
            throw std::runtime_error("Constructor: adjacency list out of range");
        bind(i.first,i.second);
    }
}

template <class T> graph<T>::~graph(){
    for(auto &node:nodes){
        arc_node *p=node->next;
        while(p!=nullptr){
            arc_node *save=p->next;
            delete p;
            p=save;
        }
        delete node;
    }
}

template <class T> void graph<T>::append(const T &data){ nodes.push_back(new vex_node(data)); }

template <class T> void graph<T>::bind(index_t vex1,index_t vex2){
    auto make_bind=[this](iterator_index_t vex1,iterator_index_t vex2){
        vex_node *p=nodes[vex1];arc_node *node=new arc_node(vex2);
        node->next=p->next;
        p->next=node;
    };
    make_bind(vex1,vex2);
    if(!directed)
        make_bind(vex2,vex1);
}

template <class T> bool graph<T>::empty() const {return nodes.empty();}

template <class T> bool graph<T>::is_directed() const {return directed;}

template <class T> void graph<T>::set_directed(bool is){directed=is;}

template <class T> T& graph<T>::operator[](index_t i){return nodes[i]->data;}

template <class T> typename graph<T>::dfs_iterator graph<T>::begin(){ 
    if(nodes.empty())
        return dfs_iterator(-1);
    auto s=new std::deque<iterator_index_t>;
    for(arc_node *ptr = nodes[0]->next; ptr != nullptr; ptr = ptr->next)
        s->push_back(ptr->data);
    return dfs_iterator(0,&nodes,s);
}

template <class T> typename graph<T>::dfs_iterator graph<T>::end(){ return dfs_iterator(-1); }

template <class T> typename graph<T>::bfs_iterator graph<T>::bfs_begin(){
    if(nodes.empty())
        return bfs_iterator(-1);
    auto q=new std::deque<iterator_index_t>;
    for(arc_node *ptr = nodes[0]->next; ptr != nullptr; ptr = ptr->next)
        q->push_back(ptr->data);
    return bfs_iterator(0,&nodes,q);
}

template <class T> typename graph<T>::bfs_iterator graph<T>::bfs_end(){ return bfs_iterator(-1); }

template <class T> typename graph<T>::topo_iterator graph<T>::topo_begin(){
    if(!directed||nodes.empty())
        return topo_iterator(-1);
    auto size=nodes.size();
    int *indegree=new int[size](),stack=-1;
    // calculate in-degree
    for(auto ptr:nodes){
        for(arc_node *node=ptr->next;node!=nullptr;node=node->next)
            indegree[node->data]++;
    }
    for(index_t i=0;i<size;i++){
        if(indegree[i]==0){
            indegree[i]=stack;
            stack=i; // stack push
        }
    }
    // cannot be sorted
    if(stack==-1)
        return topo_iterator(-1);
    else{
        int save=stack; // stack top
        stack=indegree[stack]; // stack pop
        return topo_iterator(save,&nodes,0,stack,indegree);
    }
}

template <class T> typename graph<T>::topo_iterator graph<T>::topo_end(){ return topo_iterator(-1); }

// class graph<T>::node<U>
template <class T> template <class U> graph<T>::node<U>::node():data(U()),next(nullptr){}

template <class T> template <class U> graph<T>::node<U>::node(const U &e):data(e),next(nullptr){}

template <class T> template <class U> graph<T>::node<U>::node(node<U> *p):data(p->data),next(nullptr){}


// class graph<T>::iterator_base
template <class T> graph<T>::iterator_base::iterator_base(iterator_index_t _index,std::vector<real_pointer> *_nodes):index(_index),nodes(_nodes){}

template <class T> typename graph<T>::iterator_base::reference graph<T>::iterator_base::operator*() const {return nodes->at(index)->data;}

template <class T> typename graph<T>::iterator_base::pointer graph<T>::iterator_base::operator->() const {return &(nodes->at(index)->data);}

template <class T> typename graph<T>::iterator_index_t graph<T>::iterator_base::get() const {return index;}

template <class T> bool graph<T>::iterator_base::operator==(const self_type &other) const {return index==other.index;}

template <class T> bool graph<T>::iterator_base::operator!=(const self_type &other) const {return index!=other.index;}


//class graph<T>::dfs_iterator
template <class T> graph<T>::dfs_iterator::dfs_iterator(iterator_index_t _index,std::vector<real_pointer> *_nodes,std::deque<iterator_index_t> *_s):iterator_base(_index,_nodes),component(_index),visited(nullptr),s(_s){
    if(_index!=-1){
        visited=std::shared_ptr<bool>(new bool[nodes->size()](),std::default_delete<bool[]>()); // set all to false, not sure if this works in all conditions
        visited.get()[_index]=true;
        if(_s==nullptr)
            s=std::shared_ptr<std::deque<iterator_index_t>>(new std::deque<iterator_index_t>);
    }
}

template <class T> typename graph<T>::dfs_iterator::self_type& graph<T>::dfs_iterator::operator++(){
    if(nodes==nullptr||s==nullptr||nodes->size()==0){
        index=-1;
        return *this;
    }
    if(index_t(component)<nodes->size())
        s->push_back(component);
    else{
        index=-1;
        return *this;
    }
    bool changed=false; // so ugly implemented
    while(!s->empty()) {
        iterator_index_t v = s->back();
        s->pop_back();
        if(!(visited.get()[v])) {
            visited.get()[v] = true;
            index=v;changed=true;
            for(arc_node *ptr = nodes->at(v)->next; ptr != nullptr; ptr = ptr->next)
                if(!visited.get()[ptr->data])
                    s->push_back(ptr->data);
            break;
        }
    }
    if(!changed){
        ++component;
        this->operator++();
    }
    return *this;
}

template <class T> typename graph<T>::dfs_iterator::self_type graph<T>::dfs_iterator::operator++(int){
    self_type i=*this;
    this->operator++();
    return i;
}

template <class T> typename graph<T>::dfs_iterator::self_type& graph<T>::dfs_iterator::operator=(const self_type &other){
    index=other.index;
    nodes=other.nodes;
    component=other.component;
    s=other.s;
    visited=other.visited;
    return *this;
}

// class graph<T>::bfs_iterator
template <class T> graph<T>::bfs_iterator::bfs_iterator(iterator_index_t _index,std::vector<real_pointer>* _nodes,std::deque<iterator_index_t> *_q):iterator_base(_index,_nodes),component(_index),visited(nullptr),q(_q){
    if(_index!=-1){
        visited=std::shared_ptr<bool>(new bool[nodes->size()](),std::default_delete<bool[]>()); // set all to false, not sure if this works in all conditions
        visited.get()[_index]=true;
        if(_q==nullptr)
            q=std::shared_ptr<std::deque<iterator_index_t>>(new std::deque<iterator_index_t>);
    }
}

template <class T> typename graph<T>::bfs_iterator::self_type& graph<T>::bfs_iterator::operator++(){
    if(nodes==nullptr||q==nullptr||nodes->size()==0){
        index=-1;
        return *this;
    }
    if(index_t(component)<nodes->size())
        q->push_back(component);
    else{
        index=-1;
        return *this;
    }
    bool changed=false;
    while(!q->empty()) {
        iterator_index_t v = q->front();
        q->pop_front();
        if(!visited.get()[v]) {
            visited.get()[v] = true;
            for(arc_node *ptr = nodes->at(v)->next; ptr != nullptr; ptr = ptr->next) {
                if(!visited.get()[ptr->data])
                    q->push_back(ptr->data);
            }
            index = v; changed = true; break;
        }
    }
    if(!changed){
        ++component;
        this->operator++();
    }
    return *this;
}

template <class T> typename graph<T>::bfs_iterator::self_type graph<T>::bfs_iterator::operator++(int){
    self_type i=*this;
    this->operator++();
    return i;
}

template <class T> typename graph<T>::bfs_iterator::self_type& graph<T>::bfs_iterator::operator=(const self_type &other){
    index=other.index;
    nodes=other.nodes;
    component=other.component;
    q=other.q;
    visited=other.visited;
    return *this;
}


// class graph<T>::topo_iterator
template <class T> graph<T>::topo_iterator::topo_iterator(iterator_index_t _index,std::vector<real_pointer> *_nodes,unsigned int _count,int _stack,int *_indegree):iterator_base(_index,_nodes),stack(_stack),count(_count){
    indegree=std::shared_ptr<int>(_indegree,std::default_delete<int[]>());
}

template <class T> typename graph<T>::topo_iterator::self_type& graph<T>::topo_iterator::operator++(){
    index=stack;
    if(stack==-1)
        return *this;
    stack=indegree.get()[stack];
    if((++count)>nodes->size())
        throw std::runtime_error("Topological sort: circle exists");
    for(arc_node *ptr=nodes[index]->next;ptr!=nullptr;ptr=ptr->next){
        if(--(indegree.get()[ptr->data])==0){
            indegree.get()[ptr->data]=stack;
            stack=ptr->data;
        }
    }
    return *this;
}

template <class T> typename graph<T>::topo_iterator::self_type graph<T>::topo_iterator::operator++(int){
    self_type i=*this;
    this->operator++();
    return i;
}

template <class T> typename graph<T>::topo_iterator::self_type& graph<T>::topo_iterator::operator=(const self_type &other){
    index=other.index;
    nodes=other.nodes;
    stack=other.stack;
    indegree=other.indegree;
    return *this;
}

#endif
/*
#include <iostream>
int main(){
    graph<int> a({0,1,2,3,4,5,6},{{0,1},{0,2},{1,3},{2,3},{2,4},{3,5},{4,5}},true);
    std::vector<int> v;
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
    a.topo_sort(op());
    std::cout<<"\nEnd";
    return 0;
}*/
