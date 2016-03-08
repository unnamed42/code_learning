#ifndef __ARRAY__
#define __ARRAY__

#include <cstring> // std::memcpy
#include <stdexcept>
#include <bits/stl_iterator_base_types.h> // std::random_access_iterator_tag

#define DEFAULT_INCREMENT 10UL

template <class T,std::size_t N> class array{
    public:
        
        class iterator_base{
            public:
                typedef iterator_base self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef T* real_pointer;
                typedef std::random_access_iterator_tag iterator_category;
                typedef int difference_type;
                explicit iterator_base(real_pointer _ptr);
                reference operator*() const;
                pointer operator->() const;
                real_pointer get() const;
                reference operator[](int) const;
                bool operator<(const self_type& other) const;
                bool operator>(const self_type& other) const;
                bool operator<=(const self_type& other) const;
                bool operator>=(const self_type& other) const;
                bool operator==(const self_type &other) const;
                bool operator!=(const self_type &other) const;
                difference_type operator-(const self_type &other) const;
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
                self_type& operator--();
                self_type operator--(int);
                self_type& operator+=(int);
                self_type operator+(int);
                self_type& operator-=(int);
                self_type operator-(int);
                self_type& operator=(const self_type &other);
                //self_type operator+(int,const self_type&); declared&&defined below
            protected:
                using iterator_base::ptr;
        };
        
    protected:
        T *base;
        std::size_t length;
        std::size_t occupied;
    public:
        explicit array(std::size_t increment=10);
        explicit array(T (&)[N]);
        explicit array(std::initializer_list<T>&&);
        ~array();
        std::size_t capacity() const noexcept;
        std::size_t size() const noexcept;
        bool empty() const;
        void resize(std::size_t size);
        void append(const T &element);
        void insert(std::size_t index,const T &element);
        void remove_elem(const T &element);
        
        iterator begin();
        iterator end();
        T& operator[](std::size_t index) const;
};

template <class T,std::size_t N> array<T,N>::array(std::size_t increment):base(nullptr),length(0),occupied(0){}

template <class T,std::size_t N> array<T,N>::array(T (&arr)[N]):base(nullptr),length(N),occupied(N){
    base=new T[N];
    std::memcpy(base,arr,N*sizeof(T));
}

template <class T,std::size_t N> array<T,N>::array(std::size_t length){
    base=new T[this->length=length]();
    occupied=0;
}

template <class T,std::size_t N> array<T,N>::array(std::initializer_list<T> &&l){
    base=new T[length=l.size()]();
    occupied=0;
    for(auto &&i:l)
        base[occupied++]=i;
}

template <class T,std::size_t N> array<T,N>::~array(){delete[] base;}

template <class T,std::size_t N> std::size_t array<T,N>::capacity() const {return length;}

template <class T,std::size_t N> std::size_t array<T,N>::size() const {return occupied;}

template <class T,std::size_t N> bool array<T,N>::empty() const {return occupied==0;}

template <class T,std::size_t N> typename array<T,N>::iterator array<T,N>::begin() {return iterator(base);}

template <class T,std::size_t N> typename array<T,N>::iterator array<T,N>::end() {return iterator(base+occupied);}

template <class T,std::size_t N> void array<T,N>::append(const T &element){
    if(occupied+1>length)
        throw std::out_of_range("Cannot append here");
    base[occupied++]=element;
}

template <class T,std::size_t N> void array<T,N>::insert(std::size_t index,const T &element){
    if(index>occupied)
        return;
    if(occupied+1>length)
        resize(2*length);
    if(index==occupied){
        base[index]=element;
        occupied++;
        return;
    }
    for(auto i=occupied;i>index;i--)
        base[i]^=base[i-1]^=base[i]^=base[i-1];
    base[index]=element;
    occupied++;
}

template <class T,std::size_t N> void array<T,N>::remove_elem(const T &element){
    for(int i=0;i<occupied;i++){
        if(base[i]==element){
            for(int j=i;j<occupied-1;j++)
                base[j]^=base[j+1]^=base[j]^=base[j+1];
            occupied--;
            i--;
        }
    }
}

template <class T,std::size_t N> T& array<T,N>::operator[](std::size_t index) const {return base[index];}


// class array<T,N>::iterator_base
template <class T,std::size_t N> array<T,N>::iterator_base::iterator_base(real_pointer _ptr):ptr(_ptr){}

template <class T,std::size_t N> typename array<T,N>::iterator_base::reference array<T,N>::iterator_base::operator*() const {return *ptr;}

template <class T,std::size_t N> typename array<T,N>::iterator_base::pointer array<T,N>::iterator_base::operator->() const {return ptr;}

template <class T,std::size_t N> typename array<T,N>::iterator_base::real_pointer array<T,N>::iterator_base::get() const {return ptr;}

template <class T,std::size_t N> typename array<T,N>::iterator_base::reference array<T,N>::iterator_base::operator[](int index) const {return ptr[index];}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator<(const self_type &other) const {return ptr<other.ptr;}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator>(const self_type &other) const {return ptr>other.ptr;}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator<=(const self_type &other) const {return ptr<=other.ptr;}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator>=(const self_type &other) const {return ptr>=other.ptr;}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator==(const self_type &other) const {return ptr==other.ptr;}

template <class T,std::size_t N> bool array<T,N>::iterator_base::operator!=(const self_type &other) const {return ptr!=other.ptr;}

template <class T,std::size_t N> typename array<T,N>::iterator_base::difference_type array<T,N>::iterator_base::operator-(const self_type &other) const {return ptr-other.ptr;}


// class array<T,N>::iterator
template <class T,std::size_t N> array<T,N>::iterator::iterator(real_pointer _ptr):iterator_base(_ptr){}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type& array<T,N>::iterator::operator++() {++ptr; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type array<T,N>::iterator::operator++(int) {self_type i=*this; ++ptr; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type& array<T,N>::iterator::operator--() {--ptr; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type array<T,N>::iterator::operator--(int) {self_type i=*this; --ptr; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type& array<T,N>::iterator::operator+=(int n) {ptr+=n; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type array<T,N>::iterator::operator+(int n) {self_type i=*this; i+=n; return i;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type& array<T,N>::iterator::operator-=(int n) {ptr-=n; return *this;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type array<T,N>::iterator::operator-(int n) {self_type i=*this; i-=n; return i;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type& array<T,N>::iterator::operator=(const self_type &other) {ptr=other.ptr;}

template <class T,std::size_t N> typename array<T,N>::iterator::self_type operator+(int n,const typename array<T,N>::iterator::self_type &it) {return it+n;}

#endif
//test code
/*
#include <iostream>

struct test{
    int a;
    test(int n=0):a(n){}
    void operator++(){a++;}
};

int main(){
    array<test> t;
    for(auto i = 0U; i < 100; i++)
        t.append(test(i));
    for(auto it=t.begin();it!=t.end();++it)
        it->operator++();
    for(auto &i : t)
        std::cout << i.a << " ";
    return 0;
}

*/
