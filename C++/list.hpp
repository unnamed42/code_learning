#ifndef __RUBBISH_LIST__
#define __RUBBISH_LIST__

#include "bits/linear/list_base.hpp"

namespace rubbish{
    
    template <class T> class list:public list_base<T>{
        private:
            typedef list_base<T> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::iterator iterator;
            typedef typename base_class::reverse_iterator reverse_iterator;
            using base_class::base_class;
    };
    
    
} // namespace rubbish

#endif // __RUBBISH_LIST__
