#ifndef __RUBBISH_ARRAY__
#define __RUBBISH_ARRAY__

#include <cstring> // std::memcpy
#include <stdexcept>
#include <initializer_list> // std::initializer_list
#include "bits/iterator.hpp"
#include "bits/sfinae.hpp"

namespace rubbish{

    template <class T,std::size_t N> class array{
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
            
            template <bool not_const> class _iterator: public iterator_base<not_const>{
                private:
                    typedef iterator_base<not_const> base_class;
                public:
                    typedef _iterator<not_const>           self_type;
                    typedef typename base_class::data_type data_type;
                    
                    explicit _iterator(const data_type &cursor):base_class(cursor) {}
                    _iterator(const self_type &other):base_class(other.m_cursor) {}
                    
                    self_type& operator++() {++m_cursor; return *this;}
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    self_type& operator--() {--m_cursor; return *this;}
                    self_type operator--(int) {auto i=*this; operator--(); return i;}
                    self_type& operator+=(long n) {m_cursor+=n; return *this;}
                    self_type operator+(long n) {auto i=*this; return i+=n;}
                    self_type& operator-=(long n) {m_cursor-=n; return *this;}
                    self_type operator-(long n) {auto i=*this; return i-=n;}
                    //self_type operator+(int,const self_type&); defined below
                private:
                    using base_class::m_cursor;
            };
            
            typedef _iterator<true>   iterator;
            typedef _iterator<false>  const_iterator;
            
        private:
            T *m_base;
            std::size_t m_occupied;   // space in use
            
        public:
            // Empty array
            array();
            
            // Construct from a given list
            explicit array(std::initializer_list<T>&&);
            
            // Construct from a given array
            explicit array(const T (&)[N]);
            
            // Construct an class array, the first `count`
            array(std::size_t count,const T &value);
            
            // Move-constructor
            array(array<T,N>&&);
            
            // Copy-constructor
            array(const array<T,N>&);
            
            // Destructor
            ~array();
            
            // Return the maximum possible number of elements
            std::size_t capacity() const noexcept;
            
            // Return the current number of elements
            std::size_t size() const noexcept;
            
            // Check if the array holds no element
            bool empty() const noexcept;
            
            // Append an element in the back, if full, throw std::out_of_range
            void push_back(const T &element);
            
            // Remove the final element, if empty, do nothing
            void pop_back();
            
            // Access the final element, should be used after emptiness check
            T& back();
            
            // Access the first element
            T& front();
            
            // Insert an element in position `index`, if full, throw std::out_of_range
            void insert(std::size_t index,const T &element);
            
            // Remove the elements whose value equals `element`
            void remove_elem(const T &element);
            
            // Return reference of the `index`-th element, do no boundary check
            T& operator[](std::size_t index) const;
            
            
            // Iterator functions
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
    };
    
} // namespace rubbish

#include "bits/array.inc"

#endif // __RUBBISH_ARRAY__
