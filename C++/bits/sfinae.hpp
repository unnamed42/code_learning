#ifndef __SFINAE__
#define __SFINAE__

namespace rubbish{
    
    // Forward-declaration area
    template <class T> struct is_arithmetic;
    
    // Base class of nearly all
    template <class T,T t> struct constant{
        constexpr static T value = t;
        constexpr operator T() const {return value;}
        constexpr T& operator()() const { return value; }
    };
    typedef constant<bool,true> true_type;
    typedef constant<bool,false> false_type;
    
    namespace helper{
        
        // Just to ensure sizeof(yes)!=sizeof(no)
        typedef char yes;
        typedef struct{char c[2];} no;
        
        struct is_class_h{
            template <class T> static yes test(int T::*);
            template <class T> static no test(...);
        };
        
        // This helper is from https://stackoverflow.com/a/2913870
        template <class Base,class Derived> struct is_base_of_h{
            operator Base*() const;
            operator Derived*();
            // Optional: split functions and custom conversion operators in two classes
            template <class T> static yes test(Derived*,T);
            static no test(Base*,int);
        };
        
        template <class T> struct is_pointer_h:public false_type{};
        template <class T> struct is_pointer_h<T*>:public true_type{};
        
        template <class T> struct is_member_pointer_h:public false_type{};
        template <class T,class U> struct is_member_pointer_h<U T::*>:public true_type{};
        
        // A very naive implementation
        template <class T> struct is_integral_h:public false_type {};
        template <> struct is_integral_h<bool>:public true_type {};
        template <> struct is_integral_h<char>:public true_type {};
        template <> struct is_integral_h<signed char>:public true_type {};
        template <> struct is_integral_h<unsigned char>:public true_type {};
        template <> struct is_integral_h<wchar_t>:public true_type {};
        template <> struct is_integral_h<char16_t>:public true_type {};
        template <> struct is_integral_h<char32_t>:public true_type {};
        template <> struct is_integral_h<short>:public true_type {};
        template <> struct is_integral_h<unsigned short>:public true_type {};
        template <> struct is_integral_h<int>:public true_type {};
        template <> struct is_integral_h<unsigned int>:public true_type {};
        template <> struct is_integral_h<long>:public true_type {};
        template <> struct is_integral_h<unsigned long>:public true_type {};
        template <> struct is_integral_h<long long>:public true_type {};
        template <> struct is_integral_h<unsigned long long>:public true_type {};
        
        template <class T> struct is_floating_point_h:public false_type {};
        template <> struct is_floating_point_h<float>:public true_type {};
        template <> struct is_floating_point_h<double>:public true_type {};
        template <> struct is_floating_point_h<long double>:public true_type {};
        
        template <class T,bool = is_arithmetic<T>::value> struct is_signed_h:public constant<bool,(T(0)>T(-1))> {};
        template <class T> struct is_signed_h<T,false>:public false_type {};
        
        template <class T,bool = is_arithmetic<T>::value> struct is_unsigned_h:public constant<bool,(T(0)<T(-1))> {};
        template <class T> struct is_unsigned_h<T,false>:public false_type {};
        
        template <class T> struct is_char_h:public false_type {};
        template <> struct is_char_h<char>:public true_type {};
        template <> struct is_char_h<signed char>:public true_type {};
        template <> struct is_char_h<unsigned char>:public true_type {};
        template <> struct is_char_h<wchar_t>:public true_type {};
        template <> struct is_char_h<char16_t>:public true_type {};
        template <> struct is_char_h<char32_t>:public true_type {};
    } // namespace helper
    
    template <bool,class T = void> struct enable_if { typedef T type; };
    template <class T> struct enable_if<false,T> {};
    
    template <class T> struct remove_reference { typedef T type; };
    template <class T> struct remove_reference<T&> { typedef T type; };
    template <class T> struct remove_reference<T&&> { typedef T type; };
    
    template <class T> struct remove_pointer { typedef T type; };
    template <class T> struct remove_pointer<T*> { typedef T type; };
    template <class T> struct remove_pointer<T* const> { typedef T type; };
    template <class T> struct remove_pointer<T* const volatile> { typedef T type; };
    
    template <class T> struct remove_const { typedef T type; };
    template <class T> struct remove_const<const T> {typedef T type;};
    
    template <class T> struct remove_volatile { typedef T type; };
    template <class T> struct remove_volatile<volatile T> {typedef T type;};
    
    // Remove the top most cv-qualifiers. Keep in mind that
    // `remove_cv<const volatile int*>::type` is still `const volatile int*`,
    // but `remove_cv<int* const volatile>::type` is `int*`.
    template <class T> struct remove_cv { typedef typename remove_const<typename remove_volatile<T>::type>::type type; };

    // Remove reference. This type alias is not part of std.
    template <class T> using rm_ref = typename remove_reference<T>::type;
    
    // Remove pointer. This type alias is not part of std.
    template <class T> using rm_ptr = typename remove_pointer<T>::type;
    
    // Remove const qualifiers. This type alias is not part of std.
    template <class T> using rm_c = typename remove_const<T>::type;
    
    // Remove volatile qualifiers. This type alias is not part of std.
    template <class T> using rm_v = typename remove_volatile<T>::type;
    
    // Remove cv-qualifiers. This type alias is not part of std.
    template <class T> using rm_cv = typename remove_cv<T>::type;
    
    // Remove cv-qualifiers and reference. This type alias is not part of std.
    // Pointer property is not removed.
    template <class T> using raw_type = typename remove_cv<typename remove_reference<T>::type>::type;
    
    template <class T,class U> struct is_same:public false_type {};
    template <class T> struct is_same<T,T>:public true_type {};
    
    template <class T> struct is_void:public is_same<void,typename remove_cv<T>::type> {};
    
    template <class T> struct is_integral:public helper::is_integral_h<typename remove_cv<T>::type> {};
    
    template <class T> struct is_floating_point:public helper::is_floating_point_h<typename remove_cv<T>::type> {};
    
    template <class T> struct is_arithmetic:public constant<bool,is_integral<T>::value||is_floating_point<T>::value> {};
    
    template <class T> struct is_signed:public helper::is_signed_h<T> {};
    
    template <class T> struct is_unsigned:public helper::is_unsigned_h<T> {};
    
    template <class T> struct is_char:public helper::is_char_h<typename remove_cv<T>::type> {};
    
    template <class T> struct is_class:public constant<bool,sizeof(helper::is_class_h::test<T>(0))==sizeof(helper::yes)> {};
    
    // `is_base_of` and its helper are from https://stackoverflow.com/a/2913870
    template <class Base,class Derived> struct is_base_of:public constant<bool,sizeof(helper::is_base_of_h<Base,Derived>::test(helper::is_base_of_h<Base,Derived>(),int()))==sizeof(helper::yes)> {};
    
    template <class T> struct is_pointer:public helper::is_pointer_h<typename remove_cv<T>::type> {};
    
    template <class T> struct is_member_pointer:public helper::is_member_pointer_h<typename remove_cv<T>::type> {};
    
} // namespace rubbish

#endif // __SFINAE__
