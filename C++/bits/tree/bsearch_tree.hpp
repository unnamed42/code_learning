#ifndef __RUBBISH_BINARY_SEARCH_TREE__
#define __RUBBISH_BINARY_SEARCH_TREE__

#include "binary_tree_base.hpp"
#include <initializer_list>
#include <bits/move.h> // std::move

namespace rubbish{
    // Minimum requirements of type `Node`:
    //   Same as requirements of `node` used in `binary_tree_base`.
    
    template <class T,class Node = rubbish::helper::binary_tree_node<T> > class bsearch_tree: public binary_tree_base<T,Node>{
        private:
            typedef binary_tree_base<T,Node> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::inorder_iterator iterator;
            typedef typename base_class::const_inorder_iterator const_iterator;

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
            void insert(T &&);
            
            // Find an element and return its location
            iterator find(const T&);
            const_iterator find(const T&) const;
            
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
    };

} // namespace rubbish

#include "bsearch_tree.cc"

#endif // __RUBBISH_BINARY_SEARCH_TREE__

