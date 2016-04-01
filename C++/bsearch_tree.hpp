#ifndef __RUBBISH_BINARY_SEARCH_TREE__
#define __RUBBISH_BINARY_SEARCH_TREE__

#include "bits/binary_tree_base.hpp"
#include <initializer_list>

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
            
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
    };

} // namespace rubbish

#include "bits/bsearch_tree.inc"

#endif // __RUBBISH_BINARY_SEARCH_TREE__

