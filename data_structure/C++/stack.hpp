#ifndef __STACK__
#define __STACK__

#include <stdexcept>
#include <cstdlib>

template <class T> class stack{
    protected:
        T *base;
        T *_top;
        unsigned int size;
    public:
        explicit stack();
        explicit stack(unsigned int _size);
        stack(stack &&);
        ~stack();
        unsigned int length() const;
        void clear();
        void resize(unsigned int _size);
        void push(const T &data);
        bool empty() const;
        T& pop();
        const T& top() const;
};
template <class T> stack<T>::stack():base(NULL),_top(NULL),size(0){}

template <class T> stack<T>::stack(unsigned int _size){base=_top=new T[size=_size]();}

template <class T> stack<T>::stack(stack &&other){
    base=other.base;
    _top=other._top;
    size=other.size;
    other.base=other._top=nullptr;
}

template <class T> stack<T>::~stack(){delete[] base;}

template <class T> unsigned int stack<T>::length() const{return _top-base;}

template <class T> bool stack<T>::empty() const{return _top==base;}

template <class T> void stack<T>::clear(){_top=base;}

template <class T> void stack<T>::resize(unsigned int _size){
    if(base==NULL)
        base=_top=new T[size=_size]();
    else{
        void *temp=std::realloc(base,_size*sizeof(T));
        if(temp==NULL)
            throw std::bad_alloc();
        base=static_cast<T*>(temp);size=_size;
    }
}

template <class T> void stack<T>::push(const T &data){
    if(_top-base>=size)
        resize(size+10);
    *(_top++)=data;
}

template <class T> T& stack<T>::pop(){
    if(_top!=base)
        return *(--_top);
    throw std::out_of_range("Stack empty");
}

template <class T> const T& stack<T>::top() const{
    if(_top!=base)
        return *(_top-1);
    throw std::out_of_range("Stack empty");
}

#endif

