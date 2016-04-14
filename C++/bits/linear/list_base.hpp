#ifndef __RUBBISH_LIST_BASE__
#define __RUBBISH_LIST_BASE__

#include <stdexcept>
#include <initializer_list> // std::initializer_list
#include <bits/move.h> // std::move
#include "../iterator.hpp"

namespace rubbish{

    namespace helper{
        template <class T> struct list_node{
            typedef list_node<T> self_type;
            
            T data;
            self_type *prev;
            self_type *next;
            
            list_node():data(T()),prev(nullptr),next(nullptr) {}
            explicit list_node(const T &elem):data(elem),prev(nullptr),next(nullptr) {}
            explicit list_node(T &&elem):data(std::move(elem)),prev(nullptr),next(nullptr) {}
            list_node(const self_type &o):data(o.data),next(o.next),prev(o.prev) {}
            list_node(self_type &&o):data(std::move(data)),next(o.next),prev(o.prev) {}
            
            self_type& operator=(const self_type &o) {data=o.data;next=o.next;prev=o.prev;return *this;}
            self_type& operator=(self_type &&o) {data=std::move(o.data);next=o.next;prev=o.prev;return *this;}
        };
    } // namespace helper
    
    template <class T,class Node = helper::list_node<T> > class list_base{
        public:
            typedef Node node;
            
            class iterator_base{
                public:
                    typedef T                                value_type;
                    typedef T&                               reference;
                    typedef T*                               pointer;
                    typedef std::bidirectional_iterator_tag  iterator_category;
                    typedef std::ptrdiff_t                   difference_type;
                    
                    typedef iterator_base                 self_type;
                    typedef typename list_base<T,Node>::node*  data_type;
                    
                    explicit iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    iterator_base(const self_type &other):m_cursor(other.m_cursor) {}
                    virtual ~iterator_base() =default;
                    
                    reference operator*() const {return m_cursor->data;}
                    pointer operator->() const {return &operator*();}
                    data_type get() const noexcept {return m_cursor;}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor;}
                protected:
                    data_type m_cursor;
            };
            
            class iterator: public iterator_base{
                private:
                    typedef iterator_base base_class;
                public:
                    typedef iterator self_type;
                    typedef typename base_class::data_type data_type;
                    
                    explicit iterator(const data_type &cursor):base_class(cursor) {}
                    iterator(const self_type &other):base_class(other.m_cursor) {}
                    
                    self_type& operator++() {m_cursor=(m_cursor==nullptr?nullptr:m_cursor->next);return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                    self_type& operator--() {m_cursor=(m_cursor==nullptr?nullptr:m_cursor->prev);return *this;}
                    self_type operator--(int) {auto i=*this;operator--();return i;}
                protected:
                    using base_class::m_cursor;
            };
            
            class reverse_iterator: public iterator_base{
                private:
                    typedef iterator_base base_class;
                public:
                    typedef reverse_iterator self_type;
                    typedef typename base_class::data_type data_type;
                    
                    explicit reverse_iterator(const data_type &cursor):base_class(cursor) {}
                    reverse_iterator(const self_type &other):base_class(other.m_cursor) {}
                    self_type& operator--() {m_cursor=(m_cursor==nullptr?nullptr:m_cursor->next);return *this;}
                    self_type operator--(int) {auto i=*this;operator--();return i;}
                    self_type& operator++() {m_cursor=(m_cursor==nullptr?nullptr:m_cursor->prev);return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                protected:
                    using base_class::m_cursor;
            };
            
        protected:
            node *m_head;
            node *m_end; // actual end of list_base
            std::size_t m_length;
        public:
            // Default initialization
            constexpr list_base();
            
            // Initialize a list_base of given length, filled by the given value.
            list_base(std::size_t,const T&);
            
            // Move-constructor
            list_base(list_base<T,Node>&&);
            
            // Copy-constructor
            list_base(const list_base<T,Node>&);
            
            // Construct from the given list_base
            list_base(std::initializer_list<T>&&);
            
            // Destructor
            virtual ~list_base();
            
            // Return the size of this list_base
            std::size_t size() const noexcept;
            
            // Reverse the whole list_base
            void reverse();
            
            // Check emptiness
            bool empty() const;
            
            // Merge sort in acsending order
            void sort();
            
            // Add an element to the end of this list
            void push_back(const T&);
            
            // Remove the element at the end of this list
            void pop_back();
            
            // Add an element to the head of this list
            void push_front(const T&);
            
            // Remove the element at the head of this list
            void pop_front();
            
            // Remove the node `it`
            void remove(iterator it);
            
            // Iterator functions
            iterator begin();
            iterator end();
            reverse_iterator rbegin();
            reverse_iterator rend();
            
            // Assignment operator to avoid warning [-Weffc++]
            list_base<T,Node>& operator=(const list_base<T,Node>&);
            list_base<T,Node>& operator=(list_base<T,Node>&&);
    };
} // namespace rubbish

#include "list_base.cc"

#endif // __RUBBISH_LIST_BASE__
