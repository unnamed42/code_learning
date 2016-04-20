template <class Node> void rubbish::helper::rbtree_left_rotate(Node *&root, Node *&ptr) {
    auto right = ptr->right;
    ptr->right = right->left;

    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    right->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right;
    else
        ptr->parent->right = right;

    right->left = ptr;
    ptr->parent = right;
}

template <class Node> void rubbish::helper::rbtree_right_rotate(Node *&root, Node *&ptr) {
    auto left = ptr->left;
    ptr->left = left->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left;
    else
        ptr->parent->right = left;

    left->right = ptr;
    ptr->parent = left;
}

template <class Node> void rubbish::helper::rbtree_insertion_fix(Node *&root, Node *&ptr) {
    Node *parent = nullptr;
    Node *grandpa = nullptr;

    while ((ptr != root) && (ptr->color != _rb_black) &&
            (ptr->parent->color == _rb_red)) {
        parent = ptr->parent;
        grandpa = ptr->parent->parent;

        //  Case : A
        //  Parent of ptr is left child of Grand-parent of ptr
        if (parent == grandpa->left) {
            Node *uncle = grandpa->right;
            // Case : 1
            // The uncle of ptr is also red, only Recoloring required
            if (uncle != nullptr && uncle->color == _rb_red) {
                grandpa->color = _rb_red;
                parent->color = _rb_black;
                uncle->color = _rb_black;
                ptr = grandpa;
            } else {
                // Case : 2
                // ptr is right child of its parent, Left-rotation required
                if (ptr == parent->right) {
                    rbtree_left_rotate(root, parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                // Case : 3
                // ptr is left child of its parent, Right-rotation required
                rbtree_right_rotate(root, grandpa);
                auto tmp=parent->color;
                parent->color=grandpa->color;
                grandpa->color=tmp;
                ptr = parent;
            }
        } else {
            // Case : B
            // Parent of ptr is right child of Grand-parent of ptr
            Node *uncle = grandpa->left;

            //  Case : 1
            //  The uncle of ptr is also red, only Recoloring required
            if ((uncle != nullptr) && (uncle->color == _rb_red)) {
                grandpa->color = _rb_red;
                parent->color = _rb_black;
                uncle->color = _rb_black;
                ptr = grandpa;
            } else {
                // Case : 2
                // ptr is left child of its parent, Right-rotation required
                if (ptr == parent->left) {
                    rbtree_right_rotate(root, parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                // Case : 3
                // ptr is right child of its parent, Left-rotation required
                rbtree_left_rotate(root, grandpa);
                auto tmp=parent->color;
                parent->color=grandpa->color;
                grandpa->color=tmp;
                ptr = parent;
            }
        }
    }
    root->color = _rb_black;
}

template <class T,class Node> constexpr rubbish::rbtree<T, Node>::rbtree():base_class(){}

template <class T,class Node> rubbish::rbtree<T, Node>::rbtree(std::initializer_list<T> &&l):base_class(){
    for(auto &&i:l)
        insert(std::move(i));
}

template <class T,class Node> rubbish::rbtree<T, Node>::rbtree(const self_type &o):base_class(o){}

template <class T,class Node> rubbish::rbtree<T, Node>::rbtree(self_type &&o):base_class(std::move(o)){}

template <class T,class Node> template <class U> typename rubbish::rbtree<T,Node>::node* rubbish::rbtree<T,Node>::insert(node* &root,U &&data) {
    if(root==nullptr)
        return root=new node(std::forward<U>(data));
    if (root->data < data){
        auto ret=insert(root->right,std::forward<U>(data));
        root->right->parent = root;
        return ret;
    } else if (root->data == data) {
        return root;
    } else {
        auto ret = insert(root->left,std::forward<U>(data));
        root->left->parent = root;
        return ret;
    }
}

template <class T,class Node> template <class U> typename rubbish::rbtree<T,Node>::iterator rubbish::rbtree<T,Node>::insert(U &&data) {
    auto ptr = insert(m_root,std::forward<U>(data));
    helper::rbtree_insertion_fix(m_root, ptr);
    return iterator(ptr);
}

template <class T,class Node> template <class U> typename rubbish::rbtree<T,Node>::iterator rubbish::rbtree<T,Node>::find(U &&data){
    auto ptr=m_root;
    while(ptr!=nullptr){
        if(ptr->data==data)
            break;
        if(ptr->data < data)
            ptr=ptr->right;
        else
            ptr=ptr->left;
    }
    return iterator(ptr);
}

template <class T,class Node> template <class U> typename rubbish::rbtree<T,Node>::const_iterator rubbish::rbtree<T,Node>::find(U &&data) const {return const_iterator(const_cast<self_type*>(this)->find(std::forward<U>(data)));}

template <class T,class Node> typename rubbish::rbtree<T,Node>::self_type& rubbish::rbtree<T,Node>::operator=(const self_type &o) {
    this->~rbtree();
    m_root=nullptr;
    base_class::copy_subtree(m_root,o.m_root);
    return *this;
}

template <class T,class Node> typename rubbish::rbtree<T,Node>::self_type& rubbish::rbtree<T,Node>::operator=(self_type &&o) {
    this->~rbtree();
    m_root=o.m_root;
    o.m_root=nullptr;
    return *this;
}

template <class T,class Node> typename rubbish::rbtree<T,Node>::iterator rubbish::rbtree<T,Node>::begin() { return base_class::inorder_begin();}

template <class T,class Node> typename rubbish::rbtree<T,Node>::iterator rubbish::rbtree<T,Node>::end() { return base_class::inorder_end();}

template <class T,class Node> typename rubbish::rbtree<T,Node>::const_iterator rubbish::rbtree<T,Node>::cbegin() const { return base_class::inorder_cbegin();}

template <class T,class Node> typename rubbish::rbtree<T,Node>::const_iterator rubbish::rbtree<T,Node>::cend() const { return base_class::inorder_cend();}
