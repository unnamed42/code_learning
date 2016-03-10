#ifndef __BINARY_TREE__
#define __BINARY_TREE__

#include <deque>
#include <memory> // std::shared_ptr
#include <bits/move.h> //std::forward
#include <initializer_list> // std::initializer_list
#include <bits/stl_iterator_base_types.h> // std::forward_iterator_tag

typedef enum {LEFT = 0, RIGHT} CHILD;

template <class T> class bi_tree {
    public:
        struct node {
            T data;
            node *left, *right;

            explicit node();
            explicit node(const node *p);
            explicit node(const T &e);
        };

        class iterator_base {
            public:
                typedef iterator_base self_type;
                typedef T value_type;
                typedef T &reference;
                typedef T *pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                typedef typename bi_tree<T>::node *real_pointer;
                explicit iterator_base(real_pointer _ptr, std::deque<real_pointer> * = nullptr);
                reference operator*() const;
                pointer operator->() const;
                real_pointer get() const;
                bool operator==(const self_type &other) const;
                bool operator!=(const self_type &other) const;
            protected:
                real_pointer ptr;
                std::shared_ptr<std::deque<real_pointer>> deque;
        };

        class preorder_iterator: public iterator_base {
            public:
                typedef preorder_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit preorder_iterator(real_pointer _ptr);
                self_type &operator++();
                self_type operator++(int);
                self_type &operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
                using iterator_base::deque;
        };

        class inorder_iterator: public iterator_base {
            public:
                typedef inorder_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit inorder_iterator(real_pointer _ptr, std::deque<real_pointer> * = nullptr);
                self_type &operator++();
                self_type operator++(int);
                self_type &operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
                using iterator_base::deque;
        };

        class postorder_iterator: public iterator_base {
            public:
                typedef postorder_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit postorder_iterator(real_pointer _ptr, std::deque<real_pointer> * = nullptr);
                self_type &operator++();
                self_type operator++(int);
                self_type &operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
                using iterator_base::deque;
        };

        class level_iterator: public iterator_base {
            public:
                typedef level_iterator self_type;
                typedef typename iterator_base::real_pointer real_pointer;
                explicit level_iterator(real_pointer _ptr);
                self_type &operator++();
                self_type operator++(int);
                self_type &operator=(const self_type &other);
            protected:
                using iterator_base::ptr;
                using iterator_base::deque;
        };

        typedef preorder_iterator iterator; // set pre-order iterator as default

    protected:
        node *root;

        // Deep-copy a tree from `src` to `dest`, recursively
        void copy_subtree(node *&dest, const node *src);

        // Return the depth of a binary tree whose root is `root`, recursively
        unsigned int depth(const node *root) const noexcept;

        // Destroy a tree whose root is `root`, recursively
        void delete_subtree(const node *root);

    public:

        // Construct a binary tree with given root(a raw pointer, by default it is nullptr).
        explicit bi_tree(node * = nullptr);

        // Construct a binary tree with given level-order serialization, `null` is the terminator
        bi_tree(std::initializer_list<T> &&v, const T &null);

        // Copy-construtor
        bi_tree(const bi_tree<T> &);
        
        // Move-construtor
        bi_tree(bi_tree<T>&&);

        // Destructor
        ~bi_tree();

        // Return depth of this tree
        unsigned int depth() const noexcept;

        // Insert this `root` to a new node as `LR` child
        void insert_parent(const T &_data, CHILD LR);

        // Insert a new node to `root` as `LR` child
        void insert_child(const T &_data, CHILD LR);


        // Iterator functions
        preorder_iterator begin();
        preorder_iterator end();
        inorder_iterator inorder_begin();
        inorder_iterator inorder_end();
        postorder_iterator postorder_begin();
        postorder_iterator postorder_end();
        level_iterator level_begin();
        level_iterator level_end();



};


//Member functions
template <class T> bi_tree<T>::bi_tree(node *_root): root(_root) {}

template <class T> bi_tree<T>::bi_tree(const bi_tree<T> &tree) {
    copy_subtree(root, tree.root);
}

template <class T> bi_tree<T>::bi_tree(bi_tree<T> &&tree) {
    root = tree.root;
    tree.root = nullptr;
}

template <class T> bi_tree<T>::bi_tree(std::initializer_list<T> &&v, const T &null): root(nullptr) {
    auto size = v.size();
    auto it = v.begin();
    if(size == 0 || *it == null)
        return;
    root = new node(*it);
    std::deque<node*> deque;
    deque.push_back(root);
    bool is_left = true;
    node *cur = nullptr;
    ++it;
    for(; it != v.end(); ++it) {
        node *ptr = nullptr;
        if(*it != null) {
            ptr = new node(*it);
            deque.push_back(ptr);
        }
        if(is_left) {
            cur = deque.front();
            deque.pop_front();
            cur->left = ptr;
            is_left = false;
        } else {
            cur->right = ptr;
            is_left = true;
        }
    }
}

template <class T> bi_tree<T>::~bi_tree() {
    delete_subtree(root);
}

template <class T> void bi_tree<T>::delete_subtree(const node *root) {
    if(root == nullptr)
        return;
    delete_subtree(root->left);
    delete_subtree(root->right);
    delete root;
}

template <class T> void bi_tree<T>::copy_subtree(node *&dest, const node *src) {
    if(src == nullptr)
        return;
    if(dest != nullptr)
        delete_subtree(dest);
    dest = new node(src);
    copy_subtree(dest->left, src->left);
    copy_subtree(dest->right, src->right);
}

template <class T> void bi_tree<T>::insert_parent(const T &_data, CHILD LR) {
    node *parent = new node(_data);
    if(LR == LEFT)
        parent->left = root;
    else
        parent->right = root;
    root = parent;
}

template <class T> void bi_tree<T>::insert_child(const T &_data, CHILD LR) {
    node *child = new node(_data);
    if(root == nullptr) {
        root = child;
        return;
    }
    if(LR == LEFT) {
        if(root->left != nullptr)
            delete_subtree(root->left);
        root->left = child;
    } else {
        if(root->right != nullptr)
            delete_subtree(root->right);
        root->right = child;
    }
}

template <class T> unsigned int bi_tree<T>::depth() const noexcept {
    return depth(root);
}

template <class T> unsigned int bi_tree<T>::depth(const node *root) const noexcept {
    if(root == nullptr)
        return 0;
    unsigned int left = depth(root->left), right = depth(root->right);
    return 1 + (left > right ? left : right);
}

template <class T> typename bi_tree<T>::preorder_iterator bi_tree<T>::begin() {
    return preorder_iterator(root);
}

template <class T> typename bi_tree<T>::preorder_iterator bi_tree<T>::end() {
    return preorder_iterator(nullptr);
}

template <class T> typename bi_tree<T>::inorder_iterator bi_tree<T>::inorder_begin() {
    if(root == nullptr)
        return inorder_iterator(nullptr);
    node *ptr = root;
    auto deque = new std::deque<node*>;
    while(ptr->left != nullptr) {
        deque->push_back(ptr);
        ptr = ptr->left;
    }
    return inorder_iterator(ptr, deque);
}

template <class T> typename bi_tree<T>::inorder_iterator bi_tree<T>::inorder_end() {
    return inorder_iterator(nullptr);
}

template <class T> typename bi_tree<T>::postorder_iterator bi_tree<T>::postorder_begin() {
    if(root == nullptr)
        return postorder_iterator(nullptr);
    node *ptr = root;
    auto deque = new std::deque<node*>;
    for(;;) {
        deque->push_back(ptr);
        if(ptr->left != nullptr)
            ptr = ptr->left;
        else if(ptr->right != nullptr)
            ptr = ptr->right;
        else {
            deque->pop_back(); // this is a leaf node as well as the very node to be iterated, should not be pushed into stack
            break;
        }
    }
    return postorder_iterator(ptr, deque);
}

template <class T> typename bi_tree<T>::postorder_iterator bi_tree<T>::postorder_end() {
    return postorder_iterator(nullptr);
}

template <class T> typename bi_tree<T>::level_iterator bi_tree<T>::level_begin() {
    return level_iterator(root);
}

template <class T> typename bi_tree<T>::level_iterator bi_tree<T>::level_end() {
    return level_iterator(nullptr);
}



// Tree node structure functions
template <class T> bi_tree<T>::node::node(): data(T()), left(nullptr), right(nullptr) {}

template <class T> bi_tree<T>::node::node(const node *p): data(p->data), left(nullptr), right(nullptr) {}

template <class T> bi_tree<T>::node::node(const T &e): data(e), left(nullptr), right(nullptr) {}



// class bi_tree<T>::iterator_base
template <class T> bi_tree<T>::iterator_base::iterator_base(real_pointer _ptr, std::deque<real_pointer> *_deque): ptr(_ptr), deque(_deque) {
    if(_ptr != nullptr && _deque == nullptr)
        deque = std::shared_ptr<std::deque<real_pointer>>(new std::deque<real_pointer>);
}

template <class T> typename bi_tree<T>::iterator_base::reference bi_tree<T>::iterator_base::operator*() const {
    return ptr->data;
}

template <class T> typename bi_tree<T>::iterator_base::pointer bi_tree<T>::iterator_base::operator->() const {
    return &ptr->data;
}

template <class T> bool bi_tree<T>::iterator_base::operator==(const self_type &other) const {
    return ptr == other.ptr;
}

template <class T> bool bi_tree<T>::iterator_base::operator!=(const self_type &other) const {
    return ptr != other.ptr;
}

template <class T> typename bi_tree<T>::iterator_base::real_pointer bi_tree<T>::iterator_base::get() const {
    return ptr;
}

//class bi_tree<T>::preorder_iterator
template <class T> bi_tree<T>::preorder_iterator::preorder_iterator(real_pointer _ptr): iterator_base(_ptr) {}

template <class T> typename bi_tree<T>::preorder_iterator::self_type &bi_tree<T>::preorder_iterator::operator++() {
    if(ptr->right != nullptr)
        deque->push_back(ptr->right);
    if(ptr->left != nullptr)
        ptr = ptr->left;
    else {
        if(deque == nullptr || deque->empty())
            ptr = nullptr;
        else {
            ptr = deque->back();
            deque->pop_back();
        }
    }
    return *this;
}

template <class T> typename bi_tree<T>::preorder_iterator::self_type bi_tree<T>::preorder_iterator::operator++(int) {
    self_type i = *this;
    this->operator++();
    return i;
}

template <class T> typename bi_tree<T>::preorder_iterator::self_type &bi_tree<T>::preorder_iterator::operator=(const self_type &other) {
    ptr = other.ptr;
    if(deque != nullptr && deque != other.deque)
        delete deque;
    deque = other.deque;
    return *this;
}

// class bi_tree<T>::inorder_iterator
template <class T> bi_tree<T>::inorder_iterator::inorder_iterator(real_pointer _ptr, std::deque<real_pointer> *_s): iterator_base(_ptr, _s) {}

template <class T> typename bi_tree<T>::inorder_iterator::self_type &bi_tree<T>::inorder_iterator::operator++() {
    if(deque == nullptr || deque->empty()) {
        ptr = nullptr;
        return *this;
    }
    ptr = deque->back();
    deque->pop_back();
    real_pointer tmp = ptr->right;
    while(tmp != nullptr) {
        deque->push_back(tmp);
        tmp = tmp->left;
    }
    return *this;
}

template <class T> typename bi_tree<T>::inorder_iterator::self_type bi_tree<T>::inorder_iterator::operator++(int) {
    self_type i = *this;
    this->operator++();
    return i;
}

template <class T> typename bi_tree<T>::inorder_iterator::self_type &bi_tree<T>::inorder_iterator::operator=(const self_type &other) {
    ptr = other.ptr;
    if(deque != nullptr && deque != other.deque)
        delete deque;
    deque = other.deque;
    return *this;
}

// class bi_tree<T>::postorder_iterator
template <class T> bi_tree<T>::postorder_iterator::postorder_iterator(real_pointer _ptr, std::deque<real_pointer> *_s): iterator_base(_ptr, _s) {}

template <class T> typename bi_tree<T>::postorder_iterator::self_type &bi_tree<T>::postorder_iterator::operator++() {
    if(deque == nullptr || deque->empty()) {
        ptr = nullptr;
        return *this;
    }
    real_pointer stack_top = deque->back(), tmp = stack_top->right;
    if(ptr == stack_top->left && tmp != nullptr) {
        for(;;) {
            deque->push_back(tmp);
            if(tmp->left != nullptr)
                tmp = tmp->left;
            else if(tmp->right != nullptr)
                tmp = tmp->right;
            else {
                deque->pop_back();
                ptr = tmp;
                break;
            }
        }
    } else {
        ptr = stack_top;
        deque->pop_back();
    }
    return *this;
}

template <class T> typename bi_tree<T>::postorder_iterator::self_type bi_tree<T>::postorder_iterator::operator++(int) {
    self_type i = *this;
    this->operator++();
    return i;
}

template <class T> typename bi_tree<T>::postorder_iterator::self_type &bi_tree<T>::postorder_iterator::operator=(const self_type &other) {
    ptr = other.ptr;
    if(deque != nullptr && deque != other.deque)
        delete deque;
    deque = other.deque;
    return *this;
}

// class bi_tree<T>::level_iterator
template <class T> bi_tree<T>::level_iterator::level_iterator(real_pointer _ptr): iterator_base(_ptr) {}

template <class T> typename bi_tree<T>::level_iterator::self_type &bi_tree<T>::level_iterator::operator++() {
    if(deque == nullptr) {
        ptr = nullptr;
        return *this;
    }
    if(ptr->left != nullptr)
        deque->push_back(ptr->left);
    if(ptr->right != nullptr)
        deque->push_back(ptr->right);
    if(deque->empty()) {
        ptr = nullptr;
    } else {
        ptr = deque->front();
        deque->pop_front();
    }
    return *this;
}

template <class T> typename bi_tree<T>::level_iterator::self_type bi_tree<T>::level_iterator::operator++(int) {
    self_type i = *this;
    this->operator++();
    return i;
}

template <class T> typename bi_tree<T>::level_iterator::self_type &bi_tree<T>::level_iterator::operator=(const self_type &other) {
    ptr = other.ptr;
    if(deque != nullptr && deque != other.deque)
        delete deque;
    deque = other.deque;
    return *this;
}

#endif

// test code
/*
#include <iostream>

int main(){
    bi_tree<int> tree({1,2,3,4,5,6,7,8,9,10},0);
    for(auto &it:tree)
        std::cout<<it<<" ";
    std::cout<<std::endl;
    for(auto it=tree.inorder_begin();it!=tree.inorder_end();++it)
        std::cout<<(*it)<<" ";
    std::cout<<std::endl;
    for(auto it=tree.postorder_begin();it!=tree.postorder_end();++it)
        std::cout<<(*it)<<" ";
    std::cout<<std::endl;
    for(auto it=tree.level_begin();it!=tree.level_end();++it)
        std::cout<<(*it)<<" ";
    return 0;
}
*/
