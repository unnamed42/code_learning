#ifndef __DEQUE__
#define __DEQUE__

#include "vector.hpp"

namespace rubbish{
    
    // Default length of deque's blocks
    constexpr std::size_t __deque_block = 512;
    
    // Default number of deque's blocks
    constexpr std::size_t __deque_base = 4;
    
    template <class T> class deque{
        private:
            vector<T*> m_map;
            T *m_start;
        public:
            deque();
            deque(const deque<T>&);
            deque(deque<T>&&);
            ~deque();
            void push_back(const T&);
            void pop_back();
            void push_front(const T&);
            void pop_front();
            T& back();
            T& front();
    };


} // namespace rubbish

#endif // __DEQUE__
