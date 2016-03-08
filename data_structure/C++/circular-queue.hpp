#ifndef __CIRCULAR_QUEUE__
#define __CIRCULAR_QUEUE__

#include <stdexcept>

template <class T> class queue{
    protected:
        T *base;
        unsigned int rear;          //index of the final logical element
        unsigned int front;         //index of the first logical element
        unsigned int size;          //length of allocated memory
        unsigned int content;       //length of used memory
    public:
        explicit queue();
        explicit queue(unsigned int length);
        queue(queue&&);
        ~queue();
        unsigned int length() const;
        void en_queue(const T &elem);
        T de_queue();
        bool empty() const;
        bool full() const;
};
template <class T> queue<T>::queue():base(nullptr),rear(0),front(0),size(0),content(0){}

template <class T> queue<T>::queue(unsigned int length):base(new T[length]()),rear(0),front(0),size(length),content(0){}

template <class T> queue<T>::queue(queue &&other){
    base=other.base;
    rear=other.rear;
    front=other.front;
    size=other.size;
    content=other.content;
    other.base=nullptr;
}

template <class T> queue<T>::~queue(){delete[] base;}

template <class T> unsigned int queue<T>::length() const{return content;}

template <class T> void queue<T>::en_queue(T elem){
    if(full())
        throw std::out_of_range("Queue full");
    base[rear]=elem;
    content++;
    rear=(rear+1)%size;
}

template <class T> T queue<T>::de_queue(){
    if(empty())
        throw std::out_of_range("Queue empty");
    T elem=base[front];
    front=(front+1)%size;
    content--;
    return elem;
}

template <class T> bool queue<T>::empty() const{return content==0;}

template <class T> bool queue<T>::full() const{return content>=size;} 

#endif
