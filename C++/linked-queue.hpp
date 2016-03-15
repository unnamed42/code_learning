#ifndef __LINKED_QUEUE__
#define __LINKED_QUEUE__

#include <stdexcept>
#include "bits/forward_list_base.hpp"

namespace rubbish{
    
    template <class T> class queue: public forward_list_base<T> {
        private:
            typedef forward_list_base<T> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::iterator iterator;
            
            explicit queue();
            
            void push(const T&);
            T front();
            void pop();
            
        private:
            using base_class::m_head;
            using base_class::m_end;
            using base_class::m_length;
    };
    
} // namespace rubbish


/*
template <class T> class queue{
    public:
        struct node{
            T data;
            node *next;
            
            explicit node();
            explicit node(const T&);
            explicit node(const node*);
        };
    protected:
        node *_front;
        node *_rear;
        std::size_t _length;
    public:
        
        // Initialize a queue, whose length is `length`. By default, the given `length` is 0
        explicit queue(std::size_t length=0);
        
        // Copy-constructor
        queue(const queue<T>&);
        
        // Move-constructor
        queue(queue<T>&&);
        
        // Destructor
        ~queue();
        
        // Return the length of this queue, O(1) time
        std::size_t length() const;
        
        // Check if the queue holds no element
        bool empty() const;
        
        // Push an element to queue rear
        void push(const T &elem);
        
        // Delete the element at queue front. If empty, throw std::out_of_range
        void pop();
        
        // Get the element at queue front. If empty, throw std::out_of_range
        T front() const;
};
// member functions
template <class T> queue<T>::queue(std::size_t length):_length(length){
    if(length==0){
        _front=_rear=nullptr;
        return;
    }
    _front=_rear=new node();
    for(;length>1;length--){
        _rear->next=new node();
        _rear=_rear->next;
    }
}

template <class T> queue<T>::queue(const queue<T> &other):_front(nullptr),_rear(nullptr),_length(other._length){
    node **ptr=&_front,*qtr=other._front;
    while(qtr!=nullptr){
        *ptr=_rear=new node(qtr);
        qtr=qtr->next;
        ptr=&((*ptr)->next);
    }
}

template <class T> queue<T>::queue(queue<T> &&other){
    _front=other._front;
    _rear=other._rear;
    _length=other._length;
    other._front=nullptr;
}

template <class T> queue<T>::~queue(){
    while(_front!=nullptr){
        _rear=_front->next;
        delete _front;
        _front=_rear;
    }
}

template <class T> void queue<T>::push(const T &elem){
    _length++;
    if(_front==nullptr){
        _front=_rear=new node(elem);
        return;
    }
    _rear->next=new node(elem);
    _rear=_rear->next;
}

template <class T> void queue<T>::pop(){
    if(_rear==nullptr)
        throw std::out_of_range("queue::pop: queue empty");
    _length--;
    if(_rear==_front){
        delete _rear;
        _front=_rear=nullptr;
        return elem;
    }
    node *save=_front->next;
    delete _front;_front=save;
}

template <class T> T queue<T>::front() const{
    if(_front==nullptr)
        throw std::out_of_range("queue::front: queue empty");
    return _front->data;
}

template <class T> std::size_t queue<T>::length() const{return _length;}

template <class T> bool queue<T>::empty() const{return _length==0;}

// class queue::node

template <class T> queue<T>::node::node():data(T()),next(nullptr){}

template <class T> queue<T>::node::node(const T &_data):data(_data),next(nullptr){}

template <class T> queue<T>::node::node(const node *p):data(p->data),next(nullptr){}

#endif
*/

#endif // __LINKED_QUEUE__
