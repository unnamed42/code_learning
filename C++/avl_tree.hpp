// Algorithms are borrowed from http://www.geeksforgeeks.org/avl-tree-set-1-insertion/
// and http://www.geeksforgeeks.org/avl-tree-set-2-deletion/

#ifndef __RUBBISH_AVL_TREE__
#define __RUBBISH_AVL_TREE__

#include <initializer_list>
#include "bits/binary_tree_base.hpp"
#include <bits/move.h> // std::forward, std::move

namespace rubbish{

    namespace helper{
        template <class T> struct avl_tree_node{
            T data;
            short height;
            avl_tree_node<T> *left,*right;
            
            avl_tree_node():data(),height(0),left(nullptr),right(nullptr) {}
            explicit avl_tree_node(const T &elem):data(elem),height(0),left(nullptr),right(nullptr) {}
            explicit avl_tree_node(T &&elem):data(std::move(elem)),height(0),left(nullptr),right(nullptr) {}
        };
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   A data member variable named `data`;
    //   A signed integral type member variable named `height`;
    //   Two pointer member variables named `left`, `right`, respectively;
    //   `Node` can be default-initialized and value-initialized.
    // Minimum requirements of `data`:
    //   Comparable using comparasion operator <, > and ==
    //   Assignable
    
    template <class T,class Node = rubbish::helper::avl_tree_node<T> > class avl_tree: public binary_tree_base<T,Node> {
        private:
            typedef binary_tree_base<T,Node> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::inorder_iterator iterator;

        protected:
            // Insert node to a tree whose root is `root`, and return its new root
            template <class U> node* insert(node* root,U&&);
            
            // Delete nodes in given tree
            template <class U> node* erase(node*,U&&);
            
        public:
            // Construct an empty tree
            avl_tree();
            
            // Construct from given list
            explicit avl_tree(std::initializer_list<T>&&);
            
            // Copy constructor
            avl_tree(const avl_tree<T,Node>&);
            
            // Move constructor
            avl_tree(avl_tree<T,Node>&&);
            
            // A destructor
            virtual ~avl_tree() = default;
            
            // Insert a node to this tree, and return its new root
            template <class U> node* insert(U&&);
            
            // Find a node and return location, if not found return `nullptr`
            template <class U> node* find(U&&) const;
            
            // Delete nodes with given value
            template <class U> void erase(U&&);
            
            
            // Iterator functions
            iterator begin();
            
            iterator end();
        private:
            using base_class::m_root;
    };
    
} // namespace rubbish

#include "bits/avl_tree.inc"

#endif // __RUBBISH_AVL_TREE__
