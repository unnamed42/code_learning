#ifndef __CONTINUOUS_CONTAINER__
#define __CONTINUOUS_CONTAINER__

#include <bits/stl_iterator_base_types.h> // std::random_access_iterator_tag

namespace rubbish{

    template <class T> class continuous_container{
        public:
            class iterator_base{
                public:
                    typedef T                               value_type;
                    typedef T*                              pointer;
                    typedef T&                              reference;
                    typedef std::random_access_iterator_tag iterator_category;
                    typedef long int                        difference_type;
                    
                    typedef iterator_base self_type;
                    typedef T* data_type;
                    
                    iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    iterator_base(const self_type &other):m_cursor(other.m_cursor){}
                    virtual ~iterator_base()=default;
                    
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                    reference operator[](std::size_t len) const {return m_cursor[len];}
                    reference operator*() const {return *m_cursor;}
                    data_type get() const {return m_cursor;}
                    
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator<(const self_type &other) const {return m_cursor<other.m_cursor;}
                    bool operator>(const self_type &other) const {return m_cursor>other.m_cursor;}
                    bool operator<=(const self_type& other) const {return !operator>(other);}
                    bool operator>=(const self_type& other) const {return !operator<(other);}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    difference_type operator-(const self_type &other) const {return m_cursor-other.m_cursor;}
                protected:
                    data_type m_cursor;
            };
            
            class iterator: public iterator_base {
                private:
                    typedef iterator_base base_class;
                public:
                    typedef iterator self_type;
                    typedef typename base_class::data_type data_type;
                    typedef typename base_class::difference_type difference_type;
                    
                    using base_class::base_class;
                    ~iterator()=default;
                    self_type& operator++() {++m_cursor;return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                    self_type& operator--() {--m_cursor;return *this;}
                    self_type operator--(int) {auto i=*this;operator--();return i;}
                    self_type& operator+=(long diff) {m_cursor+=diff;return *this;}
                    self_type operator+(long diff) const {return self_type(m_cursor+diff);}
                    self_type& operator-=(long diff) {m_cursor-=diff;return *this;}
                    self_type operator-(long diff) const {return self_type(m_cursor-diff);}
                protected:
                    using base_class::m_cursor;
            };
            
            class reverse_iterator: public iterator_base{
                private:
                    typedef iterator_base base_class;
                public:
                    typedef reverse_iterator self_type;
                    using base_class::base_class;
                    ~reverse_iterator()=default;
                    self_type& operator++() {--m_cursor;return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                    self_type& operator--() {++m_cursor;return *this;}
                    self_type operator--(int) {auto i=*this;operator--();return i;}
                    self_type& operator+=(long diff) {m_cursor-=diff;return *this;}
                    self_type operator+(long diff) const {return self_type(m_cursor-diff);}
                    self_type& operator-=(long diff) {m_cursor+=diff;return *this;}
                    self_type operator-(long diff) const {return self_type(m_cursor+diff);}
                protected:
                    using base_class::m_cursor;
            };
            
        protected:
            T *m_base;
            std::size_t m_size;
            std::size_t m_used;
        public:
            continuous_container(std::size_t count,const T &value=T());
            
            virtual ~continuous_container() {delete[] m_base;}
            
            iterator begin() {return iterator(m_base);}
            iterator end() {return iterator(m_base+m_used);}
    };
    
} // namespace rubbish

#include "continuous_container.inc"

#endif // __CONTINUOUS_CONTAINER__
/*
#include <iostream>

int main(){
    using namespace rubbish;
    continuous_container<int> x(10,2);
    for(auto &i:x)
        std::cout<<i<<" ";
    return 0;
}*/
