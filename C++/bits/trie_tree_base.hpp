#include <iostream>

#ifndef __RUBBISH_TRIE__BASE__
#define __RUBBISH_TRIE__BASE__

#include "../map.hpp"
#include "sfinae.hpp" // type checks
#include "pair.hpp"
#include <bits/move.h> // std::move

#define NULL_TERMINATOR '\0'

namespace rubbish{
    
    namespace helper{
        
        template <class Char> struct trie_node{
            Char data;
            map<const Char,trie_node<Char>*> children;
            
            trie_node():data(NULL_TERMINATOR),children() {}
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
    template <class Char,class Node = helper::trie_node<Char>,bool = is_char<Char>::value> class trie_tree_base{
        public:
            typedef Node node;
            
            explicit trie_tree_base(const Char* =nullptr);
            
            virtual ~trie_tree_base() = default;
            
            void insert(const Char*);
            
            // Check if this tree contains the given string, 
            // will return `true` if string pointer is `nullptr` or is an empty string.
            bool has_str(const Char*) const;
            
        protected:
            node m_root;
    };
    // Other types are invalid.
    template <class Char,class Node> class trie_tree_base<Char,Node,false> {};
    
    
    typedef trie_tree_base<char> trie_tree;
    
} // namespace rubbish

template <class Char,class Node,bool is_char> bool rubbish::trie_tree_base<Char,Node,is_char>::has_str(const Char *str) const {
    if(str==nullptr)
        return true;
    auto trie_node=&m_root;
    while(*str!=NULL_TERMINATOR){
        auto &&children=trie_node->children;
        auto &&tmp=children.find(*str);
        if(tmp==children.cend())
            return false;
        trie_node=tmp->second;
        std::cout<<trie_node->data<<" ";
        ++str;
    }
    std::cout<<std::endl;
    return true;
}

template <class Char,class Node,bool is_char> rubbish::trie_tree_base<Char,Node,is_char>::trie_tree_base(const Char *str):m_root(NULL_TERMINATOR) {insert(str);}

template <class Char,class Node,bool is_char> void rubbish::trie_tree_base<Char,Node,is_char>::insert(const Char *str){
    if(str==nullptr||*str==NULL_TERMINATOR)
        return;
    auto trie_node=&m_root;
    while(*str!=NULL_TERMINATOR){
        auto &&tmp=trie_node->children.insert({*str,nullptr});
        std::cout<<tmp->first<<" ";
        trie_node=tmp->second;
        ++str;
    }
    std::cout<<std::endl;
}

#include "trie_tree_base.inc"

#undef NULL_TERMINATOR

#endif // __RUBBISH_TRIE__BASE__
