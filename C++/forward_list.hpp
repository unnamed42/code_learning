#ifndef __FORWARD_LIST__
#define __FORWARD_LIST__

#include "bits/forward_list_base.hpp"

namespace rubbish{
    template <class T> class forward_list: public forward_list_base<T>{
        private:
            typedef forward_list_base<T> base_class;
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

#endif // __FORWARD_LIST__
