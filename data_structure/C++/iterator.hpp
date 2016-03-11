#ifndef __ITERATOR_HEADER__
#define __ITERATOR_HEADER__

#include <bits/stl_iterator_base_types.h> // for iterator tags in std

namespace rubbish{
// DataType: a variant of type 1DataType1 is really stored
// inside this class,usually the same as `Pointer`.

// T: the base of return type of deferenced iterators
template <class T,class DataType,class Tag = std::forward_iterator_tag,
          class ValueType = T,class Reference = T&,class Pointer = T*>
class iterator_base{
    public:
        typedef iterator_base<T,DataType,Tag,ValueType,Reference,Pointer> self_type;
        typedef ValueType value_type;
        typedef Reference reference;
        typedef Pointer   pointer;
        typedef long int  difference_type;
        typedef Tag       iterator_category;
        explicit iterator_base(const DataType _ptr);
        iterator_base(const self_type &other);
        reference operator*() const;
        pointer operator->() const;
        DataType get() const;
        bool operator==(const self_type &other) const;
        bool operator!=(const self_type &other) const;
        self_type& operator=(const self_type &other);
    protected:
        DataType ptr;
};

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::iterator_base(const DataType _ptr):ptr(_ptr) {}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::iterator_base(const self_type &other):ptr(other.ptr) {}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> DataType  iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::get() const {return ptr;}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> typename iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::pointer iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::operator->() const {return ptr;}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> typename iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::reference  iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::operator*() const {return ptr;}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> bool iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::operator==(const self_type &other) const {return ptr==other.ptr;}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> bool iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::operator!=(const self_type &other) const {return ptr!=other.ptr;}

template <class T,class DataType,class Tag,class ValueType,class Reference,class Pointer> typename iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::self_type& iterator_base<T,DataType,Tag,ValueType,Reference,Pointer>::operator=(const self_type &other){ ptr=other.ptr; return *this;}

}; // namespace rubbish

#endif
