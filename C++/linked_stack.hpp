#ifndef __RUBBISH_LINKED_STACK__
#define __RUBBISH_LINKED_STACK__

#include <stdexcept>
#include "list.hpp" // for default Container
#include <bits/move.h> // std::move

namespace rubbish{
    
    // Minimum requirements of type `Container`:
    //   Can push an element to its end by member function `void push_back(const T&)`
    //   Can destruct the element at its end by member function `void pop_back()`;
    //   Can access its final element by dereferencing `rbegin()`;
    //   Can return its length by member function `std::size_t size()`;
    //   Copy-constructible, move-constructible;
    //   Can be default-initialized.
    
    template <class T,class Container = rubbish::list<T> > class stack{
        private:
            Container m_base;
            
        public:
            typedef typename Container::iterator iterator;
            
            // Initialize an empty stack
            stack();
            
            // TODO: Copy-constructor
            stack(const stack<T,Container>&);
            
            // Move-constructor
            stack(stack<T,Container>&&);
            
            // Destructor
            ~stack()=default;
            
            // Return the length of this stack
            std::size_t size() const;
            
            // Check if the stack holds no element
            bool empty() const;
            
            // Push an element to stack top
            void push(const T &elem);
            
            // Delete the element at stack top. If empty, throw std::out_of_range
            void pop();
            
            // Get the element at stack top. If empty, throw std::out_of_range
            T top() const;
            
            // Iterator functions
            iterator begin();
            
            iterator end();
    };
} // namespace rubbish

#include "bits/linked_stack.inc"

#endif // __RUBBISH_LINKED_STACK__
