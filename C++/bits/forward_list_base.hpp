#ifndef __FORWARD_LIST_BASE__
#define __FORWARD_LIST_BASE__

#include <stdexcept>
#include <initializer_list> // std::initializer_list
#include <bits/move.h> // std::move
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

namespace rubbish{

    
    namespace helper{
        template <class T> struct forward_list_node{
            T data;
            forward_list_node<T>* next;
            
            forward_list_node():data(T()),next(nullptr) {}
            explicit forward_list_node(const T &elem):data(elem),next(nullptr) {}
            explicit forward_list_node(T &&elem):data(std::move(elem)),next(nullptr) {}
        };
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   A data member variable named `data`;
    //   A pointer member variable named `next`;
    //   `Node` can be default-initialized and value-initialized.
    
    template <class T,class Node = rubbish::helper::forward_list_node<T> > class forward_list_base{
        public:
            typedef Node node;
            
            class iterator_base{
                public:
                    typedef T                           value_type;
                    typedef T&                          reference;
                    typedef T*                          pointer;
                    typedef std::forward_iterator_tag   iterator_category;
                    typedef std::ptrdiff_t              difference_type;
                    
                    typedef iterator_base                   self_type;
                    typedef typename forward_list_base<T>::node* data_type;
                    
                    explicit iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    iterator_base(const self_type &other):m_cursor(other.m_cursor) {}
                    virtual ~iterator_base() = default;
                    
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
                    typedef typename iterator_base::data_type data_type;
                    
                    explicit iterator(const data_type &cursor):base_class(cursor) {}
                    iterator(const self_type &other):base_class(other.m_cursor) {}
                    
                    self_type& operator++() {m_cursor=(m_cursor==nullptr?nullptr:m_cursor->next);return *this;}
                    self_type operator++(int) {auto i=*this;operator++();return i;}
                private:
                    using base_class::m_cursor;
            };
            
        protected:
            node *m_head;
            node *m_end;// the actual final node of forward_list_base
            std::size_t m_length;
        public:
            // Initialize a forward_list_base of given length, filled by the given value. By default, it is an empty forward_list_base
            forward_list_base(std::size_t=0,const T& =T());
            
            // Move-constructor
            forward_list_base(forward_list_base<T>&&);
            
            // Copy-constructor
            forward_list_base(const forward_list_base<T>&);
            
            // Construct from the given forward_list_base
            explicit forward_list_base(std::initializer_list<T>&&);
            
            // Destructor
            virtual ~forward_list_base();
            
            // Return the size of this forward_list_base
            std::size_t size() const noexcept;
            
            // Reverse the whole forward_list_base
            void reverse();
            
            // Sort this list in ascending order, using merge sort method
            void sort();
            
            // Add an element to the end of this list
            void push_back(const T&);
            
            // Remove the element at the end of this list
            // cannot be accomplished by a singly-linked list
            // void pop_back();
            
            // Add an element to the head of this list
            void push_front(const T&);
            
            // Remove the element at the head of this list
            void pop_front();
            
            // Get the element at the front
            T& front() const;
            
            // Get the element at the back
            T& back() const;
            
            // Remove the node after `it`
            void remove_after(iterator);
            
            // Remove elements that equals `elem`
            void remove_elem(const T &elem);
            
            // Insert element after node `it`
            void insert_after(iterator,const T&);
            
            // Search the first element that equals `data`
            iterator search(const T&) const;
            
            // Iterator functions
            iterator begin();
            
            iterator end();
            
            // deleted copy assignment operator
            forward_list_base<T>& operator=(const forward_list_base<T>&) =delete;
    };

} // namespace rubbish

#include "forward_list_base.inc"

#endif // __FORWARD_LIST_BASE__
