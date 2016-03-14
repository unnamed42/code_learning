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
        std::size_t _length;
    public:
        // Initialize a stack, whose length is `length`. By default, the given `length` is 0
        explicit stack(std::size_t length=0);
        
        // Copy-constructor
        stack(const stack<T>&);
        
        // Move-constructor
        stack(stack<T>&&);
        
        // Destructor
        ~stack();
        
        // Return the length of this stack, O(1) time
        std::size_t length() const;
        
        // Check if the stack holds no element
        bool empty() const;
        
        // Push an element to stack top
        void push(const T &elem);
        
        // Delete the element at stack top. If empty, throw std::out_of_range
        void pop();
        
        // Get the element at stack top. If empty, throw std::out_of_range
        T top() const;
};

template <class T> stack<T>::stack(std::size_t length):_top(nullptr),_length(length){
    if(length==0)
        return;
    _top=new node();
    node *temp=_top;
    for(;length>1;length--){
        temp->next=new node();
        temp=temp->next;
    }
}

template <class T> stack<T>::stack(const stack<T> &other):_top(nullptr),_length(other._length){
    node **ptr=&_top,*qtr=other._top;
    while(qtr!=nullptr){
        *ptr=_rear=new node(qtr);
        qtr=qtr->next;
        ptr=&((*ptr)->next);
    }
}

template <class T> stack<T>::stack(stack<T> &&other){
    _top=other._top;
    _length=other._length;
    other._top=nullptr;
}

template <class T> stack<T>::~stack(){
    node *save;
    while(_top!=nullptr){
        save=_top->next;
        delete _top;
        _top=save;
    }
}

template <class T> std::size_t stack<T>::length() const {return _length;}

template <class T> void stack<T>::push(const T &data){
    node *p=new node(data);
    p->next=_top;
    _top=p;
    _length++;
}

template <class T> void stack<T>::pop(){
    if(_top==nullptr)
        throw std::out_of_range("stack::pop: stack empty");
    node *save=_top->next;
    delete _top;_top=save;_length--;
}

template <class T> T stack<T>::top() const{
    if(_top==nullptr)
        throw std::out_of_range("stack::pop: stack empty");
    return _top->data;
}

#endif
