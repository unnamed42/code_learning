#ifndef __RUBBISH_FORWARD_LIST__
#define __RUBBISH_FORWARD_LIST__

#include "bits/linear/forward_list_base.hpp"

namespace rubbish{
    template <class T,class Node = rubbish::helper::forward_list_node<T> > class forward_list: public forward_list_base<T,Node>{
        private:
            typedef forward_list_base<T,Node> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::iterator iterator;
            using base_class::base_class;
        private:
            using base_class::m_head;
            using base_class::m_end;
            using base_class::m_length;
    };
} // namespace rubbish

#endif // __RUBBISH_FORWARD_LIST__
