#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include "binary_tree_base.hpp"

namespace rubbish{
    template <class T> class binary_tree: public binary_tree_base<T>{
        private:
            typedef binary_tree_base<T> base_class;
            typedef typename base_class::preorder_iterator iterator;
        public:
            using base_class::base_class;
            using base_class::m_root;
            
            iterator begin() {return iterator(m_root);}
            iterator end() {return iterator(nullptr);}
    };
} // namespace rubbish

#endif // __BINARY_TREE__
