#ifndef __LIST__
#define __LIST__

#include "bits/list_base.hpp"

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

#endif // __LIST__
