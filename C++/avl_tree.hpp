// Algorithms are borrowed from http://www.geeksforgeeks.org/avl-tree-set-1-insertion/
// and http://www.geeksforgeeks.org/avl-tree-set-2-deletion/

#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <initializer_list>
#include "bits/binary_tree_base.hpp"

namespace rubbish{

    template <class T> struct avl_tree_node{
        T data;
        short height;
        avl_tree_node<T> *left,*right;
                
        explicit avl_tree_node():data(T()),height(0),left(nullptr),right(nullptr) {}
        explicit avl_tree_node(const T &elem):data(elem),height(0),left(nullptr),right(nullptr) {}
    };
    
    // Minimum requirements of type `node`:
    //   A data member variable named `data`;
    //   A signed integral type member variable named `height`;
    //   Two pointer member variables named `left`, `right`, respectively;
    //   `node` can be default-initialized and value-initialized.
    
    template <class T,class node = rubbish::avl_tree_node<T> > class avl_tree: public binary_tree_base<T,node> {
        private:
            typedef binary_tree_base<T,node> base_class;
        public:
            typedef typename base_class::inorder_iterator iterator;


        private:
            // Left rotate tree whose root is `root` 
            const node* left_rotate(node* &root);
            
            // Right rotate tree whose root is `root`
            const node* right_rotate(node* &root);
            
            // Insert node to a tree whose root is `root`, and return its location
            const node* insert(node* &root,const T&);
            
            // Delete nodes in given tree
            void erase(node*&,const T&);
            
        public:
            // Construct an empty tree
            explicit avl_tree();
            
            // Construct from given list
            explicit avl_tree(std::initializer_list<T>&&);
            
            // Copy constructor
            avl_tree(const avl_tree<T,node>&);
            
            // Move constructor
            avl_tree(avl_tree<T,node>&&);
            
            // Insert a node to this tree, and return its location
            const node* insert(const T&);
            
            // Find a node and return location, if not found return `nullptr`
            const node* find(const T&) const;
            
            // Delete nodes with given value
            void erase(const T&);
            
            // Iterator functions
            iterator begin();
            
            iterator end();
        private:
            using base_class::m_root;
    };
    
} // namespace rubbish

#include "bits/avl_tree.inc"

#endif // __AVL_TREE__


/*
// test code
#include <iostream>
using namespace rubbish;

int main(){
    std::initializer_list<int> &&p={1,1,1,2,56,9,44,100,9999999};
    avl_tree<int> tree({1,1,1,2,56,9,44,100,9999999});
    for(auto &i:tree)
        std::cout<<i<<" ";
    for(auto &&i:p){
        tree.erase(i);
        for(auto &i:tree)
            std::cout<<i<<" ";
        std::cout<<std::endl;
    }
    return 0;
}

*/
