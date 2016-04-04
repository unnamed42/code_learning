#ifndef __RUBBISH_CONTINUOUS_CONTAINER__
#define __RUBBISH_CONTINUOUS_CONTAINER__

#include <initializer_list> // std::initializer_list
#include "iterator.hpp"
#include "sfinae.hpp"

namespace rubbish{

    template <class T> class continuous_container{
        public:
            template <bool not_const> class iterator_base:public iterator<std::random_access_iterator_tag,typename condition<not_const,T,const T>::type>{
                private:
                    typedef iterator<std::random_access_iterator_tag,typename condition<not_const,T,const T>::type> base_class;
                public:
                    typedef typename base_class::pointer         pointer;
                    typedef typename base_class::reference       reference;
                    typedef typename base_class::difference_type difference_type;
                    
                    typedef iterator_base<not_const>     self_type;
                    typedef typename base_class::pointer data_type;
                    
                    explicit iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    iterator_base(const self_type &other):m_cursor(other.m_cursor) {}
                    virtual ~iterator_base() = default;
                    
                    reference operator*() const {return *m_cursor;}
                    pointer operator->() const {return &operator*();}
                    data_type get() const {return m_cursor;}
                    reference operator[](long n) const {return m_cursor[n];}
                    bool operator<(const self_type& other) const {return m_cursor<other.m_cursor;}
                    bool operator>(const self_type& other) const {return m_cursor>other.m_cursor;}
                    bool operator<=(const self_type& other) const {return !operator>(other);}
                    bool operator>=(const self_type& other) const {return !operator<(other);}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    difference_type operator-(const self_type &other) const {return m_cursor-other.m_cursor;}
                protected:
                    data_type m_cursor;
            };
            
            template <bool not_const> class _iterator: public iterator_base<not_const> {
                private:
                    typedef iterator_base<not_const> base_class;
                public:
                    typedef _iterator self_type;
                    typedef typename base_class::data_type data_type;
                    typedef typename base_class::difference_type difference_type;
                    
                    using base_class::base_class;
                    
                    self_type& operator++() {++m_cursor;return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                    self_type& operator--() {--m_cursor;return *this;}
                    self_type operator--(int) {auto i=*this;operator--();return i;}
                    self_type& operator+=(const difference_type &diff) {m_cursor+=diff;return *this;}
                    self_type operator+(const difference_type &diff) const {return self_type(m_cursor+diff);}
                    self_type& operator-=(const difference_type &diff) {m_cursor-=diff;return *this;}
                    self_type operator-(const difference_type &diff) const {return self_type(m_cursor-diff);}
                protected:
                    using base_class::m_cursor;
            };
            
            typedef _iterator<true>  iterator;
            typedef _iterator<false> const_iterator;
            
            typedef rubbish::reverse_iterator<iterator>       reverse_iterator;
            typedef rubbish::reverse_iterator<const_iterator> const_reverse_iterator;
            
        protected:
            T *m_base;
            std::size_t m_size;
            std::size_t m_used;
        public:
            continuous_container();
            
            continuous_container(continuous_container<T>&&);
            
            continuous_container(const continuous_container<T>&);
            
            continuous_container(std::size_t count,const T &value=T());
            
            explicit continuous_container(std::initializer_list<T>&&);
            
            virtual ~continuous_container();
            
            std::size_t capacity() const noexcept;
            
            std::size_t size() const noexcept;
            
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
            reverse_iterator rbegin();
            reverse_iterator rend();
            const_reverse_iterator crbegin() const;
            const_reverse_iterator crend() const;
            
            continuous_container<T>& operator=(const continuous_container<T>&);
    };
    
} // namespace rubbish

#include "continuous_container.inc"

#endif // __RUBBISH_CONTINUOUS_CONTAINER__
