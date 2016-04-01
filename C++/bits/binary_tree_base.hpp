#ifndef __RUBBISH_BINARY_TREE_BASE__
#define __RUBBISH_BINARY_TREE_BASE__

#include <deque> // stack, queue 
#include <memory> // std::shared_ptr
#include <initializer_list> // std::initializer_list
#include "iterator.hpp"
#include "sfinae.hpp"

namespace rubbish{

    typedef enum {LEFT = 0, RIGHT} CHILD;

    namespace helper{
        template <class T> struct binary_tree_node{
            T data;
            binary_tree_node<T> *left,*right;
            binary_tree_node<T> *parent;
            
            binary_tree_node():data(T()),left(nullptr),right(nullptr),parent(nullptr) {}
            explicit binary_tree_node(const T &_data):data(_data),left(nullptr),right(nullptr),parent(nullptr) {}
            explicit binary_tree_node(T &&_data):data(std::move(_data)),left(nullptr),right(nullptr),parent(nullptr) {}
        };
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   A data member variable named `data`;
    //   Three pointer member variables named `left`, `right`, `parent` respectively;
    //   `Node` can be default-initialized and value-initialized.
    
    template <class T,class Node = helper::binary_tree_node<T> > class binary_tree_base{
        public:
            typedef Node node;
            
            #include "binary_tree_iterators.inc"
            
        protected:
            // Deep-copy a tree from `src` to `dest`, recursively
            void copy_subtree(node* &dest, const node *src);
            
            // Return the depth of a binary tree whose root is `root`, recursively
            unsigned int depth(const node *root) const;
            
            // Destroy a tree whose root is `root`, recursively
            void delete_subtree(const node *root);
            
            // Data member, root of tree
            node *m_root;
        public:
            // Construct a binary tree with given root(a raw pointer, by default it is nullptr).
            explicit binary_tree_base(node * = nullptr);
            
            // Construct a binary tree with given level-order serialization, `null` means "this node is NULL"
            binary_tree_base(std::initializer_list<T> &&v, const T &null);
            
            // Construct from pre-order and in-order serialization
            binary_tree_base(std::initializer_list<T> &&pre,std::initializer_list<T> &&in);
            
            // Copy-construtor
            binary_tree_base(const binary_tree_base<T,Node>&);
            
            // Move-construtor
            binary_tree_base(binary_tree_base<T,Node>&&);
            
            // Destructor
            virtual ~binary_tree_base();
            
            // Return depth of this tree
            unsigned int depth() const;
            
            // Reset to an empty tree
            void clear();
            
            // Check emptiness
            bool empty() const noexcept;
            
            // Insert `root` to a new node as `LR` child
            void insert_parent(const T &_data, CHILD LR);
            
            // Insert a new node to `root` as `LR` child
            void insert_child(const T &_data, CHILD LR);
            
            
            // Iterator functions
            preorder_iterator preorder_begin() {return preorder_iterator(m_root);}
            preorder_iterator preorder_end() {return preorder_iterator(nullptr);}
            inorder_iterator inorder_begin() {return inorder_iterator(m_root);}
            inorder_iterator inorder_end() {return inorder_iterator(nullptr);}
            postorder_iterator postorder_begin() {return postorder_iterator(m_root);}
            postorder_iterator postorder_end() {return postorder_iterator(nullptr);}
            level_iterator level_begin() {return level_iterator(m_root);}
            level_iterator level_end() {return level_iterator(nullptr);}
            const_preorder_iterator preorder_cbegin() const {return const_preorder_iterator(m_root);}
            const_preorder_iterator preorder_cend() const {return const_preorder_iterator(nullptr);}
            const_inorder_iterator inorder_cbegin() const {return const_inorder_iterator(m_root);}
            const_inorder_iterator inorder_cend() const {return const_inorder_iterator(nullptr);}
            const_postorder_iterator postorder_cbegin() const {return const_postorder_iterator(m_root);}
            const_postorder_iterator postorder_cend() const {return const_postorder_iterator(nullptr);}
            const_level_iterator level_cbegin() const {return const_level_iterator(m_root);}
            const_level_iterator level_cend() const {return const_level_iterator(nullptr);}
            
            // Copy assignment operator
            binary_tree_base<T,Node>& operator=(const binary_tree_base<T,Node>&);
    };

} // namespace rubbish

#include "binary_tree_base.inc"

using rubbish::CHILD::LEFT;
using rubbish::CHILD::RIGHT;

#endif // __RUBBISH_BINARY_TREE_BASE__
