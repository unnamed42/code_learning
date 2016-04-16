#ifndef __RUBBISH_CONTINUOUS_CONTAINER__
#define __RUBBISH_CONTINUOUS_CONTAINER__

#include <initializer_list> // std::initializer_list
#include "../iterator.hpp"
#include "../type_traits/basic_traits.hpp"

namespace rubbish{

    template <class T> class continuous_container{
        public:
            class iterator_base:public iterator<std::random_access_iterator_tag,T>{
                private:
                    typedef iterator<std::random_accessiterator_tag,T> base_class;
                public:
                    typedef typename base_class::pointer         pointer;
                    typedef typename base_class::reference       reference;
                    typedef typename base_class::difference_type difference_type;
                    
                    typedef iterator_base                self_type;
                    typedef typename base_class::pointer data_type;
                    
                    explicit iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    iterator_base(const self_type &other):m_cursor(other.m_cursor) {}
                    virtual ~iterator_base() {}
                    
                    reference operator*() const {return *m_cursor;}
                    pointer operator->() const {return &operator*();}
                    data_type get() const {return m_cursor;}
                    reference operator[](difference_type n) const {return m_cursor[n];}
                    bool operator<(const self_type& other) const {return m_cursor<other.m_cursor;}
                    bool operator>(const self_type& other) const {return m_cursor>other.m_cursor;}
                    bool operator<=(const self_type& other) const {return !operator>(other);}
                    bool operator>=(const self_type& other) const {return !operator<(other);}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    difference_type operator-(const self_type &other) const {return m_cursor-other.m_cursor;}
                    
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor;}
                protected:
                    data_type m_cursor;
            };
            
            class iterator: public iterator_base {
                private:
                    typedef iterator_base base_class;
                public:
                    typedef iterator                             self_type;
                    typedef typename base_class::data_type       data_type;
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
            
            typedef rubbish::const_iterator<iterator>         const_iterator;
            typedef rubbish::reverse_iterator<iterator>       reverse_iterator;
            typedef rubbish::const_iterator<reverse_iterator> const_reverse_iterator;
            
            typedef continuous_container<T> self_type;
            
        protected:
            T *m_base;
            std::size_t m_size;
            std::size_t m_used;
        public:
            constexpr continuous_container();
            
            continuous_container(self_type&&);
            
            continuous_container(const self_type&);
            
            continuous_container(std::size_t count,const T &value);
            
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
            
            self_type& operator=(const self_type&);
            self_type& operator=(self_type&&);
    };
    
} // namespace rubbish

#include "continuous_container.cc"

#endif // __RUBBISH_CONTINUOUS_CONTAINER__
