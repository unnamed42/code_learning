#ifndef __MAP__
#define __MAP__

#include "avl_tree.hpp"
#include "bits/pair.hpp"
#include <bits/move.h> // std::forward, std::move
#include <initializer_list>

namespace rubbish{
    
    // Interfaces to make T1 comparable with rubbish::pair<T1,T2> (such comparasion is used in avl_tree)
    template <class T1,class T2> bool operator<(const T1 &lhs,const pair<T1,T2> &rhs){return lhs<rhs.first;}
    template <class T1,class T2> bool operator==(const T1 &lhs,const pair<T1,T2> &rhs){return lhs==rhs.first;}
    template <class T1,class T2> bool operator>(const T1 &lhs,const pair<T1,T2> &rhs){return lhs>rhs.first;}
    
    template <class T1,class T2> class map{
        public:
            typedef pair<T1,T2> value_type;
        private:
            typedef avl_tree<value_type> base_class;
            typedef typename base_class::node node;
        public:
            typedef typename base_class::iterator iterator;
            
            // Default constructor
            map();
            
            // Copy constructor
            map(const map<T1,T2>&);
            
            // Move constructor
            map(map<T1,T2>&&);
            
            // Initialize with given list
            map(std::initializer_list<value_type>&&l);
            
            // The templates below are universal reference wrappers
            
            // Insert a node
            template <class value> void insert(value&&);
            
            // Find a node whose `first` equals key
            template <class key> value_type* find(key&&) const;
            
            // Delete nodes that equal to given value
            template <class key> void erase(key&&);
            
            // Iterator functions
            iterator begin();
            
            iterator end();
            
        private:
            base_class m_base;
    };
    
} // namespace rubbish



#include "bits/map.inc"

#endif // __MAP__
