#ifndef __RUBBISH_WRAPPER__
#define __RUBBISH_WRAPPER__

#include "sfinae.hpp"
#include <cstddef>

#define HAS_DETECTOR(func_name)\
template <class T,class ReturnType,class ...Args> struct has_##func_name{\
    template <class U,ReturnType (U::*)(Args...)> struct sfinae;\
    template <class U,ReturnType (U::*)(Args...) const> struct sfinae_const;\
    template <class U> static yes test(sfinae<U,&U::func_name>*);\
    template <class U> static yes test(sfinae_const<U,&U::func_name>*);\
    template <class U> static no test(...);\
    constexpr static bool value = sizeof(test<T>(0))==sizeof(yes);\
}

namespace rubbish{
    
    namespace helper{
        
        HAS_DETECTOR(sort);
        template <class T,bool=has_sort<T,void>::value> struct sort_w{
            static void sort(T &obj) { obj.sort(); }
        };
        template <class T> struct sort_w<T,false>{
            static void sort(...) {}
        };
        
        HAS_DETECTOR(empty);
        template <class T,bool=has_empty<T,bool>::value> struct empty_w{
            static bool empty(T &obj) {return obj.empty();}
        };
        template <class T> struct empty_w<T,false>{
            static bool empty(T &obj) {return obj==T();}
        };
        
        
    } // namespace helper
    
    template <class T> void sort(T &obj) { helper::sort_w<T>::sort(obj); }
    
    template <class T> bool empty(T &obj) {return helper::empty_w<T>::empty();}
    
} // namespace rubbish

#undef HAS_DETECTOR

#endif // __RUBBISH_WRAPPER__
