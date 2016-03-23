#ifndef __ARRAY__
#define __ARRAY__

#include <cstring> // std::memcpy
#include <stdexcept>
#include <initializer_list> // std::initializer_list
#include <bits/stl_iterator_base_types.h> // std::random_access_iterator_tag

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
        T *m_base;
        std::size_t m_occupied;   // space in use
    public:
        
        // Construct from a given list
        explicit array(std::initializer_list<T>&&);
        
        // Construct from a given array
        explicit array(const T (&)[N]);
        
        // Construct an class array, the first `count`
        array(std::size_t count,const T &value);
        
        // Move-constructor
        array(array<T,N>&&);
        
        // Copy-constructor
        array(const array<T,N>&);
        
        // Destructor
        ~array();
        
        // Return the maximum possible number of elements
        std::size_t capacity() const noexcept;
        
        // Return the current number of elements
        std::size_t size() const noexcept;
        
        // Check if the array holds no element
        bool empty() const noexcept;
        
        // Append an element in the back, if full, throw std::out_of_range
        void append(const T &element);
        
        // Insert an element in position `index`, if full, throw std::out_of_range
        void insert(std::size_t index,const T &element);
        
        // Remove the elements whose value equals `element`
        void remove_elem(const T &element);
        
        // Return reference of the `index`-th element, do no boundary check
        T& operator[](std::size_t index) const;
        
        
        // Iterator functions
        iterator begin();
        iterator end();
};

template <class T,std::size_t N> array<T,N>::array(std::size_t count,const T &value):m_base(new T[N]),m_occupied(0){
    if(count>N)
        count=N;
    for(;m_occupied<=count;++m_occupied)
        m_base[m_occupied]=value;
}

template <class T,std::size_t N> array<T,N>::array(const T (&arr)[N]):m_base(nullptr),m_occupied(N){
    m_base=new T[N];
    std::memcpy(m_base,arr,N*sizeof(T));
}

template <class T,std::size_t N> array<T,N>::array(array<T,N> &&arr) {
    m_base = arr.m_base;
    m_occupied = arr.m_occupied;
    arr.m_base = nullptr;
}

template <class T,std::size_t N> array<T,N>::array(const array<T,N> &arr) {
    m_base = new T[N];
    m_occupied = arr.m_occupied;
    std::memcpy(m_base,arr.m_base,m_occupied*sizeof(T));
}

template <class T,std::size_t N> array<T,N>::array(std::initializer_list<T> &&l){
    m_base=new T[l.size()]();
    m_occupied=0;
    for(auto &&i:l)
        m_base[m_occupied++]=i;
}

template <class T,std::size_t N> array<T,N>::~array(){delete[] m_base;}

template <class T,std::size_t N> std::size_t array<T,N>::capacity() const noexcept {return N;}

template <class T,std::size_t N> std::size_t array<T,N>::size() const noexcept {return m_occupied;}

template <class T,std::size_t N> bool array<T,N>::empty() const noexcept {return m_occupied==0;}

template <class T,std::size_t N> typename array<T,N>::iterator array<T,N>::begin() {return iterator(m_base);}

template <class T,std::size_t N> typename array<T,N>::iterator array<T,N>::end() {return iterator(m_base+m_occupied);}

template <class T,std::size_t N> void array<T,N>::append(const T &element){
    if(m_occupied+1>N)
        throw std::out_of_range("array::append: array is full");
    m_base[m_occupied++]=element;
}

template <class T,std::size_t N> void array<T,N>::insert(std::size_t index,const T &element){
    if(m_occupied+1>N||index>m_occupied)
        throw std::out_of_range("array::insert: array is full");
    for(auto i=m_occupied;i>index;i--)
        m_base[i]^=m_base[i-1]^=m_base[i]^=m_base[i-1];
    m_base[index]=element;
    m_occupied++;
}

template <class T,std::size_t N> void array<T,N>::remove_elem(const T &element){
    for(auto i=0UL;i<m_occupied;i++){
        if(m_base[i]==element){
            for(auto j=i;j<m_occupied-1;j++)
                m_base[j]^=m_base[j+1]^=m_base[j]^=m_base[j+1];
            m_occupied--;
            i--;
        }
    }
}

template <class T,std::size_t N> T& array<T,N>::operator[](std::size_t index) const {return m_base[index];}


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
