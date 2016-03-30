#ifndef __RUBBISH_LINKED_QUEUE__
#define __RUBBISH_LINKED_QUEUE__

#include <stdexcept>
#include <bits/move.h> // std::move
#include "forward_list.hpp"

namespace rubbish{
    
    // Minimum requirements of type `Container`:
    //   Can push an element to its end by member function `void push_back(const T&)`
    //   Can destruct the element at its front by member function `void pop_front()`;
    //   Can access its first element by dereferencing `begin()`;
    //   Can return its length by member function `std::size_t size()`;
    //   Copy-constructible, move-constructible;
    //   Can be default-initialized.
    
    template <class T,class Container = rubbish::forward_list<T> > class queue{
        private:
            Container m_base;
        public:
            typedef typename Container::iterator iterator;
            
            // Initialize an empty queue
            queue();
            
            // TODO: Copy-constructor
            queue(const queue<T>&);
            
            // Move-constructor
            queue(queue<T>&&);
            
            // Destructor
            ~queue()=default;
            
            // Return the length of this queue
            std::size_t size() const;
            
            // Check if the queue holds no element
            bool empty() const;
            
            // Push an element to queue back
            void push(const T &elem);
            
            // Delete the element at queue front. If empty, throw std::out_of_range
            void pop();
            
            // Get the element at queue front. If empty, throw std::out_of_range
            T front() const;
    };
    
} // namespace rubbish

#endif // __RUBBISH_LINKED_QUEUE__
