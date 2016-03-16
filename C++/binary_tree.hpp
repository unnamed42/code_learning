#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include "bits/binary_tree_base.hpp"

namespace rubbish{
    // Minimum requirements of type `node`:
    //   Same as requirements of `node` used in `binary_tree_base`.
    
    template <class T,class node = rubbish::basic_tree_node<T> > class binary_tree: public binary_tree_base<T,node>{
        private:
            typedef binary_tree_base<T,node> base_class;
            typedef typename base_class::preorder_iterator iterator;
        public:
            using base_class::base_class;
            using base_class::m_root;
            
            iterator begin() {return iterator(m_root);}
            iterator end() {return iterator(nullptr);}
    };
} // namespace rubbish

#endif // __BINARY_TREE__
