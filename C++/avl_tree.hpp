#ifndef __RUBBISH_AVL_TREE__
#define __RUBBISH_AVL_TREE__

#include <initializer_list>
#include "bits/binary_tree_base.hpp"
#include <bits/move.h> // std::forward, std::move

namespace rubbish{
    
    namespace helper{
        template <class T> struct avl_tree_node{
            T data;
            avl_tree_node<T> *left,*right,*parent;
            int height;
            
            avl_tree_node():data(),left(nullptr),right(nullptr),parent(nullptr),height(1) {}
            explicit avl_tree_node(const T &elem):data(elem),left(nullptr),right(nullptr),parent(nullptr),height(1) {}
            explicit avl_tree_node(T &&elem):data(std::move(elem)),left(nullptr),right(nullptr),parent(nullptr),height(1) {}
            // Should I copy full information from `o`?
            avl_tree_node(const avl_tree_node<T> &o):data(o.data),left(o.left),right(o.right),parent(o.parent),height(o.height) {}
            avl_tree_node(avl_tree_node<T> &&o):data(std::move(o.data)),left(o.left),right(o.right),parent(o.parent),height(o.height) {}
        };
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   Satisfies requirements of `Node` used in `binary_tree_base`;
    //   Has a member of signed integral type named `height`;
    // Minimum requirements of `data`:
    //   Comparable using comparasion operator <, > and ==
    //   Assignable
    
    template <class T,class Node = rubbish::helper::avl_tree_node<T> > class avl_tree: public binary_tree_base<T,Node> {
        private:
            typedef binary_tree_base<T,Node> base_class;
        public:
            typedef typename base_class::node node;
            typedef typename base_class::inorder_iterator iterator;
            typedef typename base_class::const_inorder_iterator const_iterator;

        protected:
            // Insert node to a tree whose root is `root`, and return its new root
            // The inserted node is returned by the third parameter
            // Cannot use rvalue reference, because this value needs to be compared
            node* insert(node* root,const T&,node*&);
            
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
            
            // Insert a node to this tree, and return its location
            // Cannot use rvalue reference, reason see above
            iterator insert(const T&);
            
            // Find a node and return location, if not found return end iterator
            template <class U> const_iterator find(U&&) const;
            template <class U> iterator find(U&&);
            
            // Delete nodes with given value
            template <class U> void erase(U&&);
            
            // Iterator functions
            iterator begin();
            iterator end();
            const_iterator cbegin() const;
            const_iterator cend() const;
            
        private:
            using base_class::m_root;
    };
    
} // namespace rubbish

#include "bits/avl_tree.inc"

#endif // __RUBBISH_AVL_TREE__
