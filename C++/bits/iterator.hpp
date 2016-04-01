#ifndef __RUBBISH_ITERATOR__
#define __RUBBISH_ITERATOR__

#include "sfinae.hpp"
#include <cstddef> // std::ptrdiff_t
#include <bits/stl_iterator_base_types.h> // std iterator tags

namespace rubbish{
    
    template<class Category,class T,class Distance = std::ptrdiff_t,
             class Pointer = T*,class Reference = T& > 
    struct iterator{
        typedef typename remove_const<T>::type value_type;
        typedef Pointer                        pointer;
        typedef Reference                      reference;
        typedef Distance                       difference_type;
        typedef Category                       iterator_category;
    };
    
    template <class Iterator> struct iterator_traits{
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::pointer           pointer;
        typedef typename Iterator::reference         reference;
        typedef typename Iterator::difference_type   difference_type;
        typedef typename Iterator::iterator_category iterator_category;
    };
    template <class T> struct iterator_traits<T*>{
        typedef T                               value_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::random_access_iterator_tag iterator_category;
    };
    template <class T> struct iterator_traits<const T*>{
        typedef T                               value_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
        typedef std::ptrdiff_t                  difference_type;
        typedef std::random_access_iterator_tag iterator_category;
    };
    
    template <class Iterator> class const_iterator{
        public:
            typedef typename iterator_traits<Iterator>::value_type        value_type;
            typedef const typename iterator_traits<Iterator>::pointer     pointer;
            typedef const typename iterator_traits<Iterator>::reference   reference;
            typedef typename iterator_traits<Iterator>::difference_type   difference_type;
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            
            
        private:
            Iterator m_iter;
    };
    
} // namespace rubbish

#endif // __RUBBISH_ITERATOR__
