#ifndef __SHARED_PTR__
#define __SHARED_PTR__

#include <cstddef> // std::size_t, std::nullptr_t
#include <bits/move.h> // std::forward

namespace rubbish{

    // helper alias
    template <class T>
    using raw_type = typename std::remove_reference< typename std::remove_cv<T>::type >::type;

    template <class T>
    using rm_ref = typename std::remove_reference<T>::type;
        
    template <class T> struct deleter{
        constexpr deleter() noexcept =default;
        
        void operator() (T *ptr) {delete ptr;}
        void operator() (std::nullptr_t) {}
    };

    template <class T> struct deleter<T*>{
        constexpr deleter() noexcept =default;
        
        void operator() (T *ptr){delete ptr;}
        void operator() (std::nullptr_t) {}
    };

    template <class T> struct deleter<T[]>{
        constexpr deleter() noexcept =default;
        
        void operator() (T *ptr) {delete[] ptr;}
    };



    template <class T,class Deleter = rubbish::deleter<raw_type<T> > >
    class shared_ptr{
        protected:
            // type aliases
            typedef shared_ptr<T,Deleter> self_type;
            // type definitions
            struct data{
                rm_ref<T>* ptr;
                std::size_t count;
            };
            
        public:
            explicit shared_ptr(T *ptr) { _data=new data; _data->ptr=ptr; _data->count=1; }
            
            constexpr explicit shared_ptr(std::nullptr_t):_data(nullptr) {}
            
            shared_ptr(const self_type &other){
                _data=other._data;
                if(_data!=nullptr)
                    ++_data->count;
            }
            
            ~shared_ptr() { reset(); }
            
            // Reset this pointer to nullptr
            void reset(){
                if(_data==nullptr)
                    return;
                if(--_data->count <= 0){
                    auto &&deleter=Deleter();
                    deleter(_data->ptr);
                    delete _data;
                    _data=nullptr;
                }
            }
            
            T& operator*(){
                if(_data==nullptr)
                    throw nullptr;
                return *(_data->ptr);
            }
            
            T* operator->(){
                if(_data==nullptr)
                    throw nullptr;
                return _data->ptr;
            }
            
            T& operator[](std::size_t n){
                if(_data==nullptr)
                    throw nullptr;
                return (_data->ptr)[n];
            }
            
            T* get() {return _data==nullptr?nullptr:_data->ptr;}
            
            Deleter get_deleter() {return Deleter();}
            
            std::size_t use_count() {return _data==nullptr?0:_data->count;}
            
            self_type& operator=(const self_type &other) {
                if(operator==(other))
                    return *this;
                reset();
                _data=other._data;
                ++_data->count;
                return *this;
            }
            
            bool operator==(T *ptr) const { return _data==nullptr?false:(_data->ptr==ptr); }
            
            bool operator==(std::nullptr_t) const {return _data==nullptr;}
            
            bool operator==(const self_type &other) const {return _data==other._data;}
            
            bool operator!=(T *ptr) const {return !operator==(ptr);}
            
            bool operator!=(std::nullptr_t) const {return _data!=nullptr;}
            
            bool operator!=(const self_type &other) const {return !operator==(other);}
            
            bool operator<(T *ptr) const {return _data==nullptr?false:(_data->ptr < ptr);}
            
            bool operator<(const self_type &other) const {return get()<other.get();}
            
            bool operator>(T *ptr) const {return _data==nullptr?false:(_data->ptr > ptr);}
            
            bool operator>(const self_type &other) const {return get()>other.get();}
            
            bool operator<=(T *ptr) const {return !operator>(ptr);}
            
            bool operator<=(const self_type &other) const {return !operator>(other);}
            
            bool operator>=(T *ptr) const {return !operator<(ptr);}
            
            bool operator>=(const self_type &other) const {return !operator<(other);}
            
        protected:
            data *_data;
    };

    template <class T,class Deleter = deleter<raw_type<T> > >
    inline shared_ptr<rm_ref<T>,Deleter> make_shared(T &&data){
        auto ptr=new raw_type<T>(std::forward<rm_ref<T> >(data));
        return shared_ptr<rm_ref<T>,Deleter>(ptr);
    }

    template <class T,class Deleter = deleter<raw_type<T> > >
    inline shared_ptr<rm_ref<T>,Deleter> make_shared(T *data){
        return shared_ptr<rm_ref<T>,Deleter>(data);
    }

}  // namespace rubbish

#endif 
