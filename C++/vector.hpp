#ifndef __VECTOR__
#define __VECTOR__

#include <bits/move.h> // std::move
#include <initializer_list> // std::initializer_list
#include "bits/continuous_container.hpp"

namespace rubbish{
    constexpr std::size_t __vec_base=16;
    
    template <class T> class vector: public continuous_container<T>{
        private:
            typedef continuous_container<T> base_class;
        public:
            // Inherit all constructors
            using base_class::base_class;
            
            typedef typename base_class::iterator iterator;
            typedef typename base_class::reverse_iterator reverse_iterator;
            
            // Remove all contents
            void clear();
            
            // Append an element to the end
            void push_back(const T&);
            
            // Remove an element at the end
            void pop_back();
            
            // Resize
            void resize();
            
            // Copy assignment
            vector<T>& operator=(const vector<T>&);
            
            // Move assignment
            vector<T>& operator=(vector<T>&&);
            
            // Subscript dereference
            T& operator[](const std::size_t);
            
        private:
            using base_class::m_base;
            using base_class::m_size;
            using base_class::m_used;
    };
    
} // namespace rubbish

#include "bits/vector.inc"

#endif // __VECTOR__

/*
// test code
#include <iostream>

int main(){
    using namespace rubbish;
    vector<int> v{0,1,2,3,45};
    
    for(auto &i:v)
        std::cout<<i<<" ";
    return 0;
}*/