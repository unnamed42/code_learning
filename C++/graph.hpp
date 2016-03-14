#ifndef __GRAPH__
#define __GRAPH__

#include <deque>
#include <vector>
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include <stdexcept>
#include <initializer_list> // std::initializer_list
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

namespace rubbish{
        
    template <class T> class graph{
        public:
            typedef std::size_t index_t;
            
            typedef long _index_t; // A type used to represent index of nodes, `-1` means "no such node". Used only by iterators.
            
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
                    typedef T                           value_type;
                    typedef T&                          reference;
                    typedef T*                          pointer;
                    typedef std::forward_iterator_tag   iterator_category;
                    typedef long int                    difference_type;
                    
                    typedef iterator_base               self_type;
                    
                    explicit iterator_base(_index_t _index,std::vector<vex_node*>* =nullptr);
                    reference operator*() const;
                    pointer operator->() const {return &operator*();}
                    _index_t get() const;
                    bool operator==(const self_type &other) const;
                    bool operator!=(const self_type &other) const;
                protected:
                    _index_t index;
                    std::vector<vex_node*> *nodes;
            };
            
            class dfs_iterator: public iterator_base{
                public:
                    typedef dfs_iterator self_type;
                    typedef typename iterator_base::data_type data_type;
                    explicit dfs_iterator(_index_t _index,std::vector<data_type>* =nullptr,std::deque<_index_t>* =nullptr);
                    self_type& operator++();
                    self_type operator++(int);
                    self_type& operator=(const self_type &other);
                protected:
                    using iterator_base::index;
                    using iterator_base::nodes;
                    _index_t component; // Set `component` to iterate over isolated nodes.
                    std::shared_ptr<bool> visited;
                    std::shared_ptr<std::deque<_index_t>> s;//stack
            };
            
            class bfs_iterator: public iterator_base{
                public:
                    typedef bfs_iterator self_type;
                    typedef typename iterator_base::data_type data_type;
                    explicit bfs_iterator(_index_t _index,std::vector<data_type>* =nullptr,std::deque<_index_t>* =nullptr);
                    self_type& operator++();
                    self_type operator++(int);
                    self_type& operator=(const self_type &other);
                protected:
                    using iterator_base::index;
                    using iterator_base::nodes;
                    _index_t component;
                    std::shared_ptr<bool> visited;
                    std::shared_ptr<std::deque<_index_t>> q;//queue
            };
            
            class topo_iterator: public iterator_base{
                public:
                    typedef topo_iterator self_type;
                    typedef typename iterator_base::data_type data_type;
                    explicit topo_iterator(_index_t _index,std::vector<data_type>* =nullptr,std::size_t count=0,long stack=-1,long *indegree=nullptr);
                    self_type& operator++();
                    self_type operator++(int);
                    self_type& operator=(const self_type &other);
                protected:
                    using iterator_base::index;
                    using iterator_base::nodes;
                    long stack;
                    std::size_t count;
                    std::shared_ptr<long> indegree;
            };
            
            typedef dfs_iterator iterator;
            
        protected:
            std::vector<vex_node*> nodes;
            bool directed;
        public:
            // Construct an empty graph, by default it's undirected graph
            explicit graph(bool=false);
            
            // Copy-constructor
            graph(const graph<T> &g);
            
            // Construct from two given lists, one is value of nodes, one is relation between nodes. This funtion does no duplicate check
            graph(std::initializer_list<T>&&,std::initializer_list<std::pair<index_t,index_t>>&&,bool=false);
            
            // Destructor
            ~graph();
            
            // Append a node
            void append(const T &data);
            
            // Check if this graph holds no element
            bool empty() const;
            
            // Return if this is a directed graph
            bool is_directed() const;
            
            // Build connection between node `vex1` and `vex2`, there's difference if this graph is directed
            void bind(index_t vex1,index_t vex2);
            
            // Return referencet to the `i`-th node
            T& operator[](index_t i);
            
            // Iterator functions
            dfs_iterator begin();
            
            dfs_iterator end();
            
            bfs_iterator bfs_begin();
            
            bfs_iterator bfs_end();
            
            topo_iterator topo_begin();
            
            topo_iterator topo_end();
    };
} // namespace rubbish

#include "bits/graph.inc"

#endif // __GRAPH__
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
