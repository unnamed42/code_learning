#ifndef __RUBBISH_BINARY_TREE_BASE__
#define __RUBBISH_BINARY_TREE_BASE__

#include <deque> // stack, queue 
#include <memory> // std::shared_ptr
#include <initializer_list> // std::initializer_list
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

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
            
            class tree_iterator_base {
                public:
                    typedef T                           value_type;
                    typedef T*                          pointer;
                    typedef T&                          reference;
                    typedef std::forward_iterator_tag   iterator_category;
                    typedef std::ptrdiff_t              difference_type;
                    
                    typedef tree_iterator_base self_type;
                    typedef node*              data_type;
                    
                    explicit tree_iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    tree_iterator_base(const tree_iterator_base &other):m_cursor(other.m_cursor) {}
                    virtual ~tree_iterator_base() = default;
                    reference operator*() const {return m_cursor->data;}
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor;}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    //self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                protected:
                    data_type m_cursor;
            };
            
            class preorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef preorder_iterator self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit preorder_iterator(const data_type &cursor):base_class(cursor) {}
                    preorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            class inorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef inorder_iterator self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit inorder_iterator(const data_type &cursor);
                    inorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            class postorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef postorder_iterator self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit postorder_iterator(const data_type &cursor);
                    postorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            // This kind of iterator is totally different from the above three kinds,
            // so do not inherit from tree_iterator_base here
            class level_iterator {
                public:
                    typedef T                           value_type;
                    typedef T*                          pointer;
                    typedef T&                          reference;
                    typedef std::forward_iterator_tag   iterator_category;
                    typedef std::ptrdiff_t              difference_type;
                    
                    typedef level_iterator self_type;
                    typedef node*          data_type;
                    
                    explicit level_iterator(const data_type &cursor);
                    level_iterator(const self_type &other):m_cursor(other.m_cursor) {}
                    ~level_iterator() = default;
                    
                    reference operator*() const {return m_cursor->front()->data;}
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor==nullptr?nullptr:m_cursor->front();}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other) const;
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                private:
                    std::shared_ptr< std::deque<data_type> > m_cursor;
            };
            
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
            
            // Copy assignment operator
            binary_tree_base<T,Node>& operator=(const binary_tree_base<T,Node>&)=delete;
    };

} // namespace rubbish

#include "binary_tree_base.inc"

using rubbish::CHILD::LEFT;
using rubbish::CHILD::RIGHT;

#endif // __RUBBISH_BINARY_TREE_BASE__
