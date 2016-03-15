#ifndef __BINARY_SEARCH_TREE__
#define __BINARY_SEARCH_TREE__

#include "bits/binary_tree_base.hpp"
#include <initializer_list>

namespace rubbish{
    template <class T> class bsearch_tree: public binary_tree_base<T>{
        private:
            typedef binary_tree_base<T> base_class;
        public:
            typedef typename base_class::node node;
            
            typedef typename base_class::inorder_iterator iterator;

        private:
            using base_class::m_root;
            
            // Insert any data to any tree
            void insert(node*&,const T&);
            
        public:
            explicit bsearch_tree(node *root=nullptr);
            explicit bsearch_tree(std::initializer_list<T>&&);
            bsearch_tree(const bsearch_tree<T>&);
            
            // Insert data to this tree
            void insert(const T&);
            
            iterator begin();
            iterator end();
    };

} // namespace rubbish

#include "bits/bsearch_tree.inc"

#endif // __BINARY_SEARCH_TREE__

/*
//test code
#include <iostream>

int main(){
    rubbish::bsearch_tree<int> tree({1,5,7,9,5,25,0,1,9,4,1});
    for(auto &i:tree)
        std::cout<<i<<" ";
    std::cout<<std::endl;
    for(auto it=tree.level_begin();it!=tree.level_end();++it)
        std::cout<<(*it)<<" ";
    std::cout<<"\nExit successfully.";
    return 0;
}
*/
