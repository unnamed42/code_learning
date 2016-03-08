#ifndef __LINKED_STACK__
#define __LINKED_STACK__

#include <stdexcept>

template <class T> class stack{
    public:
        struct node{
            T data;
            node *next;
            explicit node():data(T()),next(nullptr){}
            explicit node(const T &_data):data(_data),next(nullptr){}
            explicit node(node *p):data(p->data),next(p->next){}
        };
    protected:
        node *_top;
        unsigned int _length;
    public:
        explicit stack();
        explicit stack(unsigned int length);
        ~stack();
        unsigned int length() const;
        void push(const T&);
        T pop();
        T top() const;
};
template <class T> stack<T>::stack():_top(nullptr),_length(0){}

template <class T> stack<T>::stack(unsigned int length){
    _top=new node();_length=length;
    node *temp=_top;
    for(;length>1;length--){
        temp->next=new node();
        temp=temp->next;
    }
    temp->next=nullptr;
}

template <class T> stack<T>::~stack(){
    node *save;
    while(_top!=nullptr){
        save=_top->next;
        delete _top;
        _top=save;
    }
}

template <class T> unsigned int stack<T>::length() const{
    return _length;
}

template <class T> void stack<T>::push(const T &data){
    node *p=new node(data);
    p->next=_top;
    _top=p;
    _length++;
}

template <class T> T stack<T>::pop(){
    if(_top==nullptr)
        throw std::out_of_range("Stack empty");
    T elem=_top->data;node *save=_top->next;
    delete _top;_top=save;_length--;
    return elem;
}

template <class T> T stack<T>::top() const{
    if(_top==nullptr)
        throw std::out_of_range("Stack empty");
    return _top->data;
}

#endif
