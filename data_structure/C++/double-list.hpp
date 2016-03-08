#ifndef __DOUBLE_LIST__
#define __DOUBLE_LIST__

#include <bits/stl_iterator_base_types.h> // std::bidirectional_iterator_tag

template <class T> class list{
    public:
        struct node{
            T data;
            node *prev;
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
                typedef std::bidirectional_iterator_tag iterator_category;
                typedef int difference_type;
                typedef typename list<T>::node* real_pointer;
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
                self_type operator++(int);
                self_type& operator++();
                self_type operator--(int);
                self_type& operator--();
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
        };
        
        class reverse_iterator: public iterator_base{
            public:
                typedef reverse_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit reverse_iterator(real_pointer _ptr);
                self_type operator++(int);
                self_type& operator++();
                self_type operator--(int);
                self_type& operator--();
                self_type& operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
        };
        
    protected:
        node *_head;
        node *_end; // actual end of list
    public:
        explicit list();
        list(list&&);
        list(const list<T>&);
        list(std::initializer_list<T>&&);
        ~list();
        void reverse();
        void append(const T&);
        void remove(iterator it);
        
        iterator begin();
        iterator end();
        reverse_iterator rbegin();
        reverse_iterator rend();
};
// member functions
template <class T> list<T>::list():_head(nullptr),_end(nullptr){}

template <class T> list<T>::list(list &&other){
    _head=other._head;
    _end=other._end;
    other._head=other._end=nullptr;
}

template <class T> list<T>::list(std::initializer_list<T> &&l){
    _head=_end=nullptr;
    if(l.size()==0)
        return;
    for(const auto &i:l){
        if(_head==nullptr)
            _head=_end=new node(i);
        else{
            _end->next=new node(i);
            _end->next->prev=_end;
            _end=_end->next;
        }
    }
}

template <class T> list<T>::list(const list<T> &l){
    node *another=l._head;
    _head=_end=new node(another);
    another=another->next;
    while(another!=nullptr){
        _end->next=new node(another);
        _end->next->prev=_end;
        _end=_end->next;
        another=another->next;
    }
}

template <class T> list<T>::~list(){
    node *save;
    while(_head!=nullptr){
        save=_head->next;
        delete _head;
        _head=save;
    }
}

template <class T> void list<T>::reverse(){
    if(_head==nullptr||_head->next==nullptr)
        return;
    _end=_head;
    node *_node=_head->next;
    while(_node->next!=nullptr){
        node *temp=_node->prev;
        _node->prev=_node->next;
        _node->next=temp;
        _node=_node->prev;
    }
    _head=_node;
}

template <class T> void list<T>::append(const T &data){
    if(_head==nullptr){
        _head=_end=new node(data);
        return;
    }
    _end->next=new node(data);
    _end->next->prev=_end;
    _end=_end->next;
}

template <class T> void list<T>::remove(iterator it){
    node *ptr=it.get();
    if(ptr==nullptr)
        return;
    if(ptr==_head && ptr==_end){
        delete ptr;
        _head=_end=nullptr;
    } else if(ptr==_head) {
        ptr=_head->next;
        delete _head;
        _head=ptr;
    } else if(ptr==_end) {
        ptr=_end->prev;
        delete _end;
        _end=ptr;
    } else {
        ptr->next->prev=ptr->prev;
        ptr->prev->next=ptr->next;
        delete ptr;
    }
}

template <class T> typename list<T>::iterator list<T>::begin() {return iterator(_head);}

template <class T> typename list<T>::iterator list<T>::end() {return iterator(nullptr);}

template <class T> typename list<T>::reverse_iterator list<T>::rbegin() {return reverse_iterator(_end);}

template <class T> typename list<T>::reverse_iterator list<T>::rend() {return reverse_iterator(nullptr);}


// class list<T>::node
template <class T> list<T>::node::node():data(T()),prev(nullptr),next(nullptr){}

template <class T> list<T>::node::node(const T &e):data(e),prev(nullptr),next(nullptr){}

template <class T> list<T>::node::node(const node *p):data(p->data),prev(nullptr),next(nullptr){}


// class list<T>::iterator_base
template <class T> list<T>::iterator_base::iterator_base(real_pointer _ptr):ptr(_ptr){}

template <class T> typename list<T>::iterator_base::reference list<T>::iterator_base::operator*() const {return ptr->data;}

template <class T> typename list<T>::iterator_base::pointer list<T>::iterator_base::operator->() const {return &ptr->data;}

template <class T> typename list<T>::iterator_base::real_pointer list<T>::iterator_base::get() const {return ptr;}

template <class T> bool list<T>::iterator_base::operator==(const self_type &other){return ptr==other.ptr;}

template <class T> bool list<T>::iterator_base::operator!=(const self_type &other){return ptr!=other.ptr;}


// class list<T>::iterator
template <class T> list<T>::iterator::iterator(real_pointer _ptr):iterator_base(_ptr){}

template <class T> typename list<T>::iterator::self_type& list<T>::iterator::operator++(){
    ptr=ptr->next;
    return *this;
}

template <class T> typename list<T>::iterator::self_type list<T>::iterator::operator++(int){
    self_type i=*this;
    ptr=ptr->next;
    return i;
}

template <class T> typename list<T>::iterator::self_type& list<T>::iterator::operator--(){
    ptr=ptr->prev;
    return *this;
}

template <class T> typename list<T>::iterator::self_type list<T>::iterator::operator--(int){
    self_type i=*this;
    ptr=ptr->prev;
    return i;
}

template <class T> typename list<T>::iterator::self_type& list<T>::iterator::operator=(const self_type &other) {ptr=other.ptr;}



// class list<T>::reverse_iterator
template <class T> list<T>::reverse_iterator::reverse_iterator(real_pointer _ptr):iterator_base(_ptr){}

template <class T> typename list<T>::reverse_iterator::self_type& list<T>::reverse_iterator::operator++(){
    ptr=ptr->prev;
    return *this;
}

template <class T> typename list<T>::reverse_iterator::self_type list<T>::reverse_iterator::operator++(int){
    self_type i=*this;
    ptr=ptr->prev;
    return i;
}

template <class T> typename list<T>::reverse_iterator::self_type& list<T>::reverse_iterator::operator--(){
    ptr=ptr->next;
    return *this;
}

template <class T> typename list<T>::reverse_iterator::self_type list<T>::reverse_iterator::operator--(int){
    self_type i=*this;
    ptr=ptr->next;
    return i;
}

template <class T> typename list<T>::reverse_iterator::self_type& list<T>::reverse_iterator::operator=(const self_type &other) {ptr=other.ptr;}
#endif
