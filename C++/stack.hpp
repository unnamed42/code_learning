#ifndef __RUBBISH_STACK__
#define __RUBBISH_STACK__

#include "deque.hpp"
#include <bits/move.h>

namespace rubbish{
    
    template <class T,class Container = rubbish::deque<T> > class stack{
        private:
            typedef Container base_class;
        public:
            typedef typename base_class::iterator iterator;
            typedef typename base_class::const_iterator const_iterator;
            typedef typename base_class::reverse_iterator reverse_iterator;
            typedef typename base_class::const_reverse_iterator const_reverse_iterator;
            
            stack();
            
            stack(const stack<T>&);
            
            stack(stack<T>&&);
            
            std::size_t size() const noexcept;
            
            bool empty() const;
            
            void push(const T&);
            
            void push(T&&);
            
            T top() const;
            
            void pop();
            
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
            reverse_iterator rbegin();
            reverse_iterator rend();
            const_reverse_iterator crbegin() const;
            const_reverse_iterator crend() const;
            
        private:
            base_class m_base;
    };
    
} // namespace rubbish

#include "bits/stack.inc"

#endif // __RUBBISH_STACK__
