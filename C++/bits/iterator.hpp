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
    
    // `Iterator` must at least satisfies BidirectionalIterator
    template <class Iterator> class reverse_iterator{
        public:
            typedef typename iterator_traits<Iterator>::value_type        value_type;
            typedef typename iterator_traits<Iterator>::pointer           pointer;
            typedef typename iterator_traits<Iterator>::reference         reference;
            typedef typename iterator_traits<Iterator>::difference_type   difference_type;
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            
            typedef reverse_iterator<Iterator> self_type;
            
            explicit reverse_iterator(const Iterator &iter):m_iter(iter) {}
            reverse_iterator(const self_type &other):m_iter(other.m_iter) {}
            virtual ~reverse_iterator {}
            
            virtual reference operator*() const {return *(m_iter-1);}
            pointer operator->() const {return &operator*();}
            reference operator[](long n) const {return m_iter[-n-1];}
            Iterator base() const {return m_iter;}
            
            self_type& operator++() {--m_iter;return *this;}
            self_type operator++(int) {auto i=*this;operator++();return i;}
            self_type& operator--() {++m_iter;return *this;}
            self_type operator--(int) {auto i=*this;operator--();return i;}
            self_type& operator+=(const difference_type &diff) {m_iter-=diff;return *this;}
            self_type operator+(const difference_type &diff) const {return self_type(m_iter-diff);}
            self_type& operator-=(const difference_type &diff) {m_iter+=diff;return *this;}
            self_type operator-(const difference_type &diff) const {return self_type(m_iter+diff);}
            
            bool operator<(const self_type &other) const {return m_iter<other.m_iter;}
            bool operator>(const self_type &other) const {return m_iter>other.m_iter;}
            bool operator<=(const self_type &other) const {return !operator>(other);}
            bool operator>=(const self_type &other) const {return !operator<(other);}
            bool operator==(const self_type &other) const {return m_iter==other.m_iter;}
            bool operator!=(const self_type &other) const {return !operator==(other);}
        private:
            Iterator m_iter;
    };
    
} // namespace rubbish

#endif // __RUBBISH_ITERATOR__
