#ifndef __RUBBISH_BINARY_TREE__
#define __RUBBISH_BINARY_TREE__

#include "bits/tree/binary_tree_base.hpp"

namespace rubbish{
    // Minimum requirements of type `node`:
    //   Same as requirements of `node` used in `binary_tree_base`.
    
    template <class T,class Node = helper::binary_tree_node<T> > class binary_tree: public binary_tree_base<T,Node>{
        private:
            typedef binary_tree_base<T,Node> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::preorder_iterator iterator;
            typedef typename base_class::const_preorder_iterator const_iterator;
            
            using base_class::base_class;
            
            iterator begin() {return iterator(m_root);}
            iterator end() {return iterator(nullptr);}
            const_iterator cbegin() const {return const_iterator(m_root);}
            const_iterator cend() const {return const_iterator(nullptr);}
        private:
            using base_class::m_root;
    };
} // namespace rubbish

#endif // __RUBBISH_BINARY_TREE__
