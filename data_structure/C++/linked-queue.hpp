#ifndef __LINKED_QUEUE__
#define __LINKED_QUEUE__

#include <stdexcept>

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
        unsigned int _length;
    public:
        explicit queue();
        explicit queue(unsigned int length);
        ~queue();
        unsigned int length() const;
        bool empty() const;
        void en_queue(const T &elem);
        T de_queue();
        T front() const;
};
// member functions
template <class T> queue<T>::queue():_front(nullptr),_rear(nullptr),_length(0){}

template <class T> queue<T>::queue(unsigned int length):_length(length){
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

template <class T> queue<T>::~queue(){
    while(_front!=nullptr){
        _rear=_front->next;
        delete _front;
        _front=_rear;
    }
}

template <class T> void queue<T>::en_queue(const T &elem){
    _length++;
    if(_front==nullptr){
        _front=_rear=new node(elem);
        return;
    }
    _rear->next=new node(elem);
    _rear=_rear->next;
}

template <class T> T queue<T>::de_queue(){
    if(_rear==nullptr)
        throw std::out_of_range("Queue empty");
    T elem=_front->data;
    _length--;
    if(_rear==_front){
        delete _rear;
        _front=_rear=nullptr;
        return elem;
    }
    node *save=_front->next;
    delete _front;_front=save;
    return elem;
}

template <class T> T queue<T>::front() const{
    if(_front==nullptr)
        throw std::out_of_range("Queue empty");
    return _front->data;
}

template <class T> unsigned int queue<T>::length() const{return _length;}

template <class T> bool queue<T>::empty() const{return _length==0;}

// class queue::node

template <class T> queue<T>::node::node():data(T()),next(nullptr){}

template <class T> queue<T>::node::node(const T &_data):data(_data),next(nullptr){}

template <class T> queue<T>::node::node(const node *p):data(p->data),next(nullptr){}

#endif
