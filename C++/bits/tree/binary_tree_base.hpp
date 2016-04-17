#ifndef __RUBBISH_BINARY_TREE_BASE__
#define __RUBBISH_BINARY_TREE_BASE__

#include <deque> // stack, queue 
#include <memory> // std::shared_ptr
#include <initializer_list> // std::initializer_list
#include "../iterator.hpp"
#include "../type_traits/basic_traits.hpp" // rubbish::condition, used in iterators

namespace rubbish{

    typedef enum {LEFT = 0, RIGHT} CHILD;

    namespace helper{
        template <class T> struct binary_tree_node{
            typedef binary_tree_node<T> self_type;
            
            T data;
            self_type *left,*right;
            self_type *parent;
            
            constexpr binary_tree_node():data(T()),left(nullptr),right(nullptr),parent(nullptr) {}
            explicit binary_tree_node(const T &_data):data(_data),left(nullptr),right(nullptr),parent(nullptr) {}
            explicit binary_tree_node(T &&_data):data(std::move(_data)),left(nullptr),right(nullptr),parent(nullptr) {}
            // Should I copy full information from `o`?
            binary_tree_node(const self_type &o):data(o.data),left(o.left),right(o.right),parent(o.parent) {}
            binary_tree_node(self_type &&o):data(std::move(o.data)),left(o.left),right(o.right),parent(o.parent) {}
            
            // Assignment operators to avoid warning [-Weffc++]
            self_type& operator=(const self_type &o) {data=o.data; left=o.left; right=o.right; parent=o.parent; return *this;}
            self_type& operator=(self_type &&o) {data=std::move(o.data); left=o.left; right=o.right; parent=o.parent; return *this;}
        };
    } // namespace helper
    
    // Minimum requirements of type `Node`:
    //   A data member variable named `data`;
    //   Three pointer member variables named `left`, `right`, `parent` respectively;
    //   `Node` can be default-initialized and value-initialized.
    
    template <class T,class Node = helper::binary_tree_node<T> > class binary_tree_base{
        public:
            typedef Node node;
            
            #include "binary_tree_iterators.cc"
            
            typedef const_iterator<preorder_iterator>  const_preorder_iterator;
            typedef const_iterator<inorder_iterator>   const_inorder_iterator;
            typedef const_iterator<postorder_iterator> const_postorder_iterator;
            typedef const_iterator<level_iterator>     const_level_iterator;
            
            typedef binary_tree_base<T,Node> self_type;
            
        protected:
            // Deep-copy a tree from `src` to `dest`, recursively
            static void copy_subtree(node* &dest, const node *src);
            
            // Return the depth of a binary tree whose root is `root`, recursively
            static unsigned int depth(const node *root);
            
            // Destroy a tree whose root is `root`, recursively
            static void delete_subtree(const node *root);
            
            // Data member, root of tree
            node *m_root;
        public:
            // Default initialization
            constexpr binary_tree_base();
            
            // Construct a binary tree with given root(a raw pointer).
            explicit binary_tree_base(node*);
            
            // Construct a binary tree with given level-order serialization, `null` means "this node is NULL"
            binary_tree_base(std::initializer_list<T> &&v, const T &null);
            
            // Construct from pre-order and in-order serialization
            binary_tree_base(std::initializer_list<T> &&pre,std::initializer_list<T> &&in);
            
            // Copy-construtor
            binary_tree_base(const self_type&);
            
            // Move-construtor
            binary_tree_base(self_type&&);
            
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
            preorder_iterator preorder_begin();
            preorder_iterator preorder_end();
            inorder_iterator inorder_begin();
            inorder_iterator inorder_end();
            postorder_iterator postorder_begin();
            postorder_iterator postorder_end();
            level_iterator level_begin();
            level_iterator level_end();
            const_preorder_iterator preorder_cbegin() const;
            const_preorder_iterator preorder_cend() const;
            const_inorder_iterator inorder_cbegin() const;
            const_inorder_iterator inorder_cend() const;
            const_postorder_iterator postorder_cbegin() const;
            const_postorder_iterator postorder_cend() const;
            const_level_iterator level_cbegin() const;
            const_level_iterator level_cend() const;
            
            // Assignment operator to avoid warning [-Weffc++]
            self_type& operator=(const self_type&);
            self_type& operator=(self_type&&);
    };
} // namespace rubbish

#include "binary_tree_base.cc"

using rubbish::CHILD::LEFT;
using rubbish::CHILD::RIGHT;

#endif // __RUBBISH_BINARY_TREE_BASE__
