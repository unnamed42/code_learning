#ifndef __LIST__
#define __LIST__

#include <stdexcept>
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

template <class T> class list{
    public:
        struct node{
            T data;
            node *next;
            explicit node();
            explicit node(const T &elem);
            explicit node(const node *p);
        };
        
        class iterator_base{
            public:
                typedef iterator_base self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef typename list<T>::node* real_pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                explicit iterator_base(real_pointer _ptr);
                reference operator*() const;
                pointer operator->() const;
                real_pointer get() const;
                bool operator==(const self_type &other);
                bool operator!=(const self_type &other);
            protected:
                real_pointer ptr;
        };
        
        class iterator: public iterator_base{
            public:
                typedef iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit iterator(real_pointer _ptr);
                self_type& operator++();
                self_type operator++(int);
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
        };
        
    protected:
        node *_head;
        node *_end;// the actual final node of list, for acceleration of append()
        unsigned int _length;
    public:
        explicit list(unsigned int=0);
        list(std::initializer_list<T>&&);
        list(list&&);
        ~list();
        void remove_elem(const T &elem);
        void remove_after(iterator);
        void insert_after(iterator,const T&);
        iterator search(const T&) const;
        void append(const T&);
        unsigned int length() const;
        void reverse();
        iterator begin();
        iterator end();
};

// member functions
template <class T> list<T>::list(unsigned int length){
    _length=length;
    if(length==0){
        _end=_head=nullptr;
        return;
    }
    _end=_head=new node;
    for(;length>1;length--){
        _end->next=new node;
        _end=_end->next;
    }
}

template <class T> list<T>::list(list &&other){
    _head=other._head;
    _end=other._end;
    _length=other._length;
    other._head=other._end=nullptr;
}

template <class T> list<T>::list(std::initializer_list<T> &&l){
    _head=_end=nullptr;
    if(l.size()==0)
        return;
    for(auto &&i:l){
        if(_head==nullptr)
            _head=_end=new node(i);
        else{
            _end->next=new node(i);
            _end=_end->next;
        }
    }
}

template <class T> list<T>::~list(){
    while(_head!=nullptr){
        _end=_head->next;
        delete _head;
        _head=_end;
    }
}

template <class T> void list<T>::remove_elem(const T &elem){
    if(_head->data==elem){
        if(_head->next==nullptr){
            delete _head;
            _length--;
            _head=nullptr;
            return;
        }
        node *ptr=_head->next;
        delete _head;
        _length--;
        _head=ptr;
        this->remove_elem(elem);
    }
    node *ptr=_head->next,*prev=_head;
    while(ptr!=nullptr){
        if(ptr->elem==elem){
            prev->next=ptr->next;
            delete ptr;
            _length--;
            ptr=prev->next;
        } else {
            prev=ptr;
            ptr=ptr->next;
        }
    }
    if(prev->next==nullptr)
        _end=prev;
}

template <class T> unsigned int list<T>::length() const {return _length;}

template <class T> typename list<T>::iterator list<T>::search(const T &elem) const{
    node *p=_head;
    while(p!=nullptr){
        if(p->data==elem)
            return iterator(p);
        p=p->next;
    }
    return iterator(nullptr);
}

template <class T> void list<T>::insert_after(iterator it,const T &elem){
    node const *_it=it.get();
    if(_it==nullptr)
        return;
    if(_it==_end){
        _end->next=new node(elem);
        _end=_end->next;
    } else {
        node *temp=new node(elem);
        temp->next=_it->next;
        _it->next=temp;
    }
}

template <class T> void list<T>::append(const T &elem){
    _length++;
    if(_head==nullptr){
        _head=_end=new node(elem);
        return;
    }
    _end->next=new node(elem);
    _end=_end->next;
}

template <class T> typename list<T>::iterator list<T>::begin(){ return iterator(_head); }

template <class T> typename list<T>::iterator list<T>::end(){ return iterator(nullptr); }

template <class T> void list<T>::reverse(){
    if(_head==nullptr||_head->next==nullptr)
        return;
    _end=_head;
    node *nhead=nullptr;
    while(_head!=nullptr){
        node *p=_head->next;
        _head->next=nhead;
        nhead=_head;
        _head=p;
    }
    _head=nhead;
}

template <class T> void list<T>::remove_after(iterator it){
    node const *_it=it.get();
    if(_it==nullptr||_it->next==nullptr)
        return;
    node *save=_it->next->next;
    delete _it->next;
    _it->next=save;
    _length--;
}


// class list<T>::node
template <class T> list<T>::node::node():data(T()),next(nullptr){}

template <class T> list<T>::node::node(const T &e):data(e),next(nullptr){}

template <class T> list<T>::node::node(const node *p):data(p->data),next(nullptr){}


// class list<T>::iterator_base
template <class T> list<T>::iterator_base::iterator_base(real_pointer _ptr):ptr(_ptr){}

template <class T> typename list<T>::iterator_base::reference list<T>::iterator_base::operator*() const { return ptr->data; }

template <class T> typename list<T>::iterator_base::pointer list<T>::iterator_base::operator->() const { return &ptr->data; }

template <class T> typename list<T>::iterator_base::real_pointer list<T>::iterator_base::get() const { return ptr; }

template <class T> bool list<T>::iterator_base::operator==(const self_type &other){ return ptr==other.ptr; }

template <class T> bool list<T>::iterator_base::operator!=(const self_type &other){ return ptr!=other.ptr; }

// class list<T>::iterator
template <class T> list<T>::iterator::iterator(real_pointer _ptr):iterator_base(_ptr){}

template <class T> typename list<T>::iterator_base::self_type& list<T>::iterator_base::operator++(){
    ptr=ptr->next;
    return *this;
}

template <class T> typename list<T>::iterator_base::self_type list<T>::iterator_base::operator++(int){
    self_type i=*this;
    ptr=ptr->next;
    return i;
}

template <class T> typename list<T>::iterator_base::self_type& list<T>::iterator_base::operator=(const self_type &other){ 
    ptr=other.ptr;
    return *this;
}

#endif
/*
#include <algorithm>
#include <iostream>

struct test{
    int data;
    test(int n):data(n){}
    void increment(){data++;}
};

int main(){
    list<test> l;
    for(int i=0;i<10;i++)
        l.append(test(i));
    for(auto &i:l)
        i.increment();
    std::for_each(l.begin(),l.end(),[](test &t){std::cout<<t.data<<" ";});
    return 0;
}
*/
