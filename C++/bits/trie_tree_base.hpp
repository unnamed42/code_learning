#include <iostream>

#ifndef __TRIE__BASE__
#define __TRIE__BASE__

#include "../avl_tree.hpp"
#include "sfinae.hpp" // type checks
#include <bits/move.h> // std::move

#define NULL_TERMINATOR '\0'

namespace rubbish{
    
    namespace helper{
        
        template <class Char> struct trie_node{
            Char data;
            avl_tree< trie_node<Char> > children;
            
            explicit trie_node():data(NULL_TERMINATOR),children() {}
            explicit trie_node(const Char &d):data(d),children() {}
            trie_node(const trie_node<Char> &other):data(other.data),children(other.children) {}
            // Provided for expensive time cost on copy of `children`
            trie_node(trie_node<Char> &&other):data(std::move(other.data)),children(std::move(other.children)) {}
            
            bool operator<(const trie_node<Char> &other) const {return data<other.data;}
            bool operator==(const trie_node<Char> &other) const {return data==other.data;}
            bool operator>(const trie_node<Char> &other) const {return data>other.data;}
        };
        
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   A data member variable named `data`;
    //   A container named `children` that supports insert and find;
    //   `Node` can be default-initialized and value-initialized;
    //   `Node` is copy-constructible and move-constructible;
    //   Some more requirements of avl_tree_node's `data` member variable.
    
    // In instantiation, only parameter `Char` of character types are expected.
    template <class Char = typename enable_if<is_char<Char>::value,Char>::type,class Node = helper::trie_node<Char> > class trie_tree_base{
        public:
            typedef Node node;
            
            explicit trie_tree_base(const Char* =nullptr);
            
            virtual ~trie_tree_base() = default;
            
            void insert(const Char*);
            
            // Check if this tree contains the given string, will return `true`
            // if string pointer is `nullptr` or is an empty string.
            bool has_str(const Char*) const;
            
        protected:
            node m_root;
    };
    
    template <> class trie_tree_base{};
    
    typedef trie_tree_base<char> trie_tree;
    
} // namespace rubbish

#include "trie_tree_base.inc"

#undef NULL_TERMINATOR

#endif // __TRIE__BASE__
