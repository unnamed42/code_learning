#ifndef __BINARY_TREE_BASE__
#define __BINARY_TREE_BASE__

#include <deque> // stack, queue 
#include <memory> // std::shared_ptr
#include <initializer_list> // std::initializer_list
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

namespace rubbish{

    typedef enum {LEFT = 0, RIGHT} CHILD;

    template <class T> struct binary_tree_node{
        T data;
        binary_tree_node<T> *left,*right;
        
        explicit binary_tree_node():data(T()),left(nullptr),right(nullptr) {}
        explicit binary_tree_node(const T &data_):data(data_),left(nullptr),right(nullptr) {}
    };
    
    // Minimum requirements of type `node`:
    //   A data member variable named `data`;
    //   Two pointer member variables named `left`, `right`, respectively;
    //   `node` can be default-initialized and value-initialized.
    
    template <class T,class node = rubbish::binary_tree_node<T> > class binary_tree_base{
        public:
            
            class tree_iterator_base {
                public:
                    typedef T                           value_type;
                    typedef T*                          pointer;
                    typedef T&                          reference;
                    typedef std::forward_iterator_tag   iterator_category;
                    typedef std::ptrdiff_t              difference_type;
                    
                    typedef tree_iterator_base                  self_type;
                    typedef std::shared_ptr<std::deque<node*> > data_type;
                    
                    explicit tree_iterator_base(node *root);
                    explicit tree_iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    tree_iterator_base(const tree_iterator_base &other):m_cursor(other.m_cursor) {}
                    virtual ~tree_iterator_base() = default;
                    virtual reference operator*() const = 0;
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor;}
                    virtual bool operator==(const self_type &other) const;
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                protected:
                    data_type m_cursor; 
            };
            
            class preorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef preorder_iterator self_type;
                    typedef typename tree_iterator_base::reference reference;
                    
                    explicit preorder_iterator(node *root);
                    preorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    reference operator*() const override {return m_cursor->back()->data;}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other)  const override {return base_class::operator==(other)&&other.m_cursor->back()==m_cursor->back();}
                private:
                    using base_class::m_cursor;
            };
            
            class inorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef inorder_iterator self_type;
                    typedef typename tree_iterator_base::reference reference;
                    explicit inorder_iterator(node *root);
                    inorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    reference operator*() const override {return m_cursor->back()->data;}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other) const override {return base_class::operator==(other)&&other.m_cursor->back()==m_cursor->back();}
                private:
                    using base_class::m_cursor;
            };
            
            class postorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef postorder_iterator self_type;
                    typedef typename tree_iterator_base::reference reference;
                    explicit postorder_iterator(node *root);
                    postorder_iterator(const self_type &other):base_class(other.m_cursor) {}
                    reference operator*() const override {return m_cursor->back()->data;}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other) const override {return base_class::operator==(other)&&other.m_cursor->back()==m_cursor->back();}
                private:
                    using base_class::m_cursor;
            };
            
            class level_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef level_iterator self_type;
                    typedef typename tree_iterator_base::reference reference;
                    explicit level_iterator(node *root);
                    level_iterator(const self_type &other):base_class(other.m_cursor) {}
                    reference operator*() const override {return m_cursor->front()->data;}
                    self_type& operator++();
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other) const override {return base_class::operator==(other)&&other.m_cursor->front()==m_cursor->front();}
                private:
                    using base_class::m_cursor;
            };
            
        protected:
            // Deep-copy a tree from `src` to `dest`, recursively
            void copy_subtree(node* &dest, const node *src);

            // Return the depth of a binary tree whose root is `root`, recursively
            unsigned int depth(const node *root) const noexcept;

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
            binary_tree_base(const binary_tree_base<T,node> &);
            
            // Move-construtor
            binary_tree_base(binary_tree_base<T,node>&&);

            // Destructor
            virtual ~binary_tree_base();

            // Return depth of this tree
            unsigned int depth() const noexcept;

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
            binary_tree_base<T,node>& operator=(const binary_tree_base<T,node>&)=delete;
    };

} // namespace rubbish

#include "binary_tree_base.inc"

using rubbish::CHILD::LEFT;
using rubbish::CHILD::RIGHT;

#endif // __BINARY_TREE_BASE__

/*
// test code
#include <iostream>
using namespace rubbish;

int main(){
    binary_tree_base<int> tree({1,2,3,4,5,6,7,8,9,10},0);
    for(auto it=tree.preorder_begin();it!=tree.preorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=tree.inorder_begin();it!=tree.inorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=tree.postorder_begin();it!=tree.postorder_end();++it)
        std::cout<<(*it)<<",";
    std::cout<<std::endl;
    for(auto it=tree.level_begin();it!=tree.level_end();++it)
        std::cout<<(*it)<<",";
    binary_tree_base<int> treea({1,2,4,8,9,5,10,3,6,7},{8,4,9,2,10,5,1,6,3,7});
    for(auto it=treea.level_begin();it!=treea.level_end();++it)
        std::cout<<(*it)<<",";
    return 0;
}
*/
