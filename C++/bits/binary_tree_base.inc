// Binary tree base class function implementions
template <class T,class node> rubbish::binary_tree_base<T,node>::binary_tree_base(node *root): m_root(root) {}

template <class T,class node> rubbish::binary_tree_base<T,node>::binary_tree_base(const binary_tree_base<T,node> &tree) {
    copy_subtree(m_root, tree.m_root);
}

template <class T,class node> rubbish::binary_tree_base<T,node>::binary_tree_base(binary_tree_base<T,node> &&tree) {
    m_root = tree.m_root;
    tree.m_root = nullptr;
}

template <class T,class node> rubbish::binary_tree_base<T,node>::binary_tree_base(std::initializer_list<T> &&pre,std::initializer_list<T> &&in):m_root(nullptr) {
    if(pre.size()!=in.size()||pre.size()==0||in.size()==0)
        return;
    std::deque<node*> stack;
    auto ipre=pre.begin();      // iterator of `pre`
    m_root = new node(*(ipre++));
    stack.push_back(m_root); 
    auto iin=in.begin();        // iterator of `in`
    for(;ipre!=pre.end();++ipre) {
        node *tmp= stack.back();
        if((tmp->data)!=*iin) {
            tmp->left = new node(*ipre);
            stack.push_back(tmp->left);
        } else {
            while(!stack.empty() && (stack.back()->data)==*iin) {
                tmp=stack.back(); stack.pop_back(); ++iin; 
            }
            // TODO: change the result of difference to some unsigned type
            if((iin-in.begin())< in.size()) {
                tmp->right = new node(*ipre);
                stack.push_back(tmp->right);
            } 
        }  
    }
}

template <class T,class node> rubbish::binary_tree_base<T,node>::binary_tree_base(std::initializer_list<T> &&v, const T &null): m_root(nullptr) {
    auto size = v.size();
    auto it = v.begin();
    if(size == 0 || *it == null)
        return;
    m_root = new node(*it);
    std::deque<node*> deque;
    deque.push_back(m_root);
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

template <class T,class node> rubbish::binary_tree_base<T,node>::~binary_tree_base() {
    delete_subtree(m_root);
}

template <class T,class node> void rubbish::binary_tree_base<T,node>::delete_subtree(const node *root) {
    if(root == nullptr)
        return;
    delete_subtree(root->left);
    delete_subtree(root->right);
    delete root;
}

template <class T,class node> void rubbish::binary_tree_base<T,node>::copy_subtree(node *&dest, const node *src) {
    if(src == nullptr){
        dest=nullptr;
        return;
    }
    if(dest != nullptr)
        delete_subtree(dest);
    dest = new node(src);
    copy_subtree(dest->left, src->left);
    copy_subtree(dest->right, src->right);
}

template <class T,class node> void rubbish::binary_tree_base<T,node>::insert_parent(const T &_data, CHILD LR) {
    node *parent = new node(_data);
    if(LR == LEFT)
        parent->left = m_root;
    else
        parent->right = m_root;
    m_root = parent;
}

template <class T,class node> void rubbish::binary_tree_base<T,node>::insert_child(const T &_data, CHILD LR) {
    node *child = new node(_data);
    if(m_root == nullptr) {
        m_root = child;
        return;
    }
    if(LR == LEFT) {
        if(m_root->left != nullptr)
            delete_subtree(m_root->left);
        m_root->left = child;
    } else {
        if(m_root->right != nullptr)
            delete_subtree(m_root->right);
        m_root->right = child;
    }
}

template <class T,class node> unsigned int rubbish::binary_tree_base<T,node>::depth() const noexcept {
    return depth(m_root);
}

template <class T,class node> unsigned int rubbish::binary_tree_base<T,node>::depth(const node *root) const noexcept {
    if(root == nullptr)
        return 0;
    unsigned int left = depth(root->left), right = depth(root->right);
    return 1 + (left > right ? left : right);
}




// Iterator function implementions


template <class T,class node> rubbish::binary_tree_base<T,node>::tree_iterator_base::tree_iterator_base(node *root):m_cursor(nullptr) {
    if(root==nullptr)
        return;
    m_cursor=std::make_shared<std::deque<node*> >(std::deque<node*>());
}

// Two iterators are equal iif they are both
// empty or their "next node"-s are equal

template <class T,class node> bool rubbish::binary_tree_base<T,node>::tree_iterator_base::operator==(const self_type &other) const {
    if(other.m_cursor==nullptr&&m_cursor==nullptr)
        return true;
    if(other.m_cursor==nullptr)
        return false;
}

template <class T,class node> rubbish::binary_tree_base<T,node>::preorder_iterator::preorder_iterator(node *root):base_class(root) {
    if(m_cursor!=nullptr)
        m_cursor->push_back(root);
}

template <class T,class node> typename rubbish::binary_tree_base<T,node>::preorder_iterator::self_type& rubbish::binary_tree_base<T,node>::preorder_iterator::operator++(){
    if(m_cursor==nullptr||m_cursor->empty()){
        m_cursor.reset();
        return *this;
    }
    auto tmp=m_cursor->back();
    m_cursor->pop_back();
    if(tmp->right != nullptr)
        m_cursor->push_back(tmp->right);
    if(tmp->left != nullptr)
        m_cursor->push_back(tmp->left);
    // Double check if the iteration comes to an end
    if(m_cursor->empty())
        m_cursor.reset();
    return *this;
}

template <class T,class node> rubbish::binary_tree_base<T,node>::inorder_iterator::inorder_iterator(node *root):base_class(root) {
    if(m_cursor==nullptr)
        return;
    while(root!=nullptr){
        m_cursor->push_back(root);
        root=root->left;
    }
}

template <class T,class node> typename rubbish::binary_tree_base<T,node>::inorder_iterator::self_type& rubbish::binary_tree_base<T,node>::inorder_iterator::operator++(){
    if(m_cursor==nullptr||m_cursor->empty()){
        m_cursor.reset();
        return *this;
    }
    auto ptr = m_cursor->back()->right;
    m_cursor->pop_back();
    while(ptr != nullptr) {
        m_cursor->push_back(ptr);
        ptr = ptr->left;
    }
    // Double check if the iteration comes to an end
    if(m_cursor->empty())
        m_cursor.reset();
    return *this;
}

template <class T,class node> rubbish::binary_tree_base<T,node>::postorder_iterator::postorder_iterator(node *root):base_class(root) {
    if(m_cursor==nullptr)
        return;
    for(;;) {
        m_cursor->push_back(root);
        if(root->left != nullptr)
            root = root->left;
        else if(root->right != nullptr)
            root = root->right;
        else
            break;
    }
}

template <class T,class node> typename rubbish::binary_tree_base<T,node>::postorder_iterator::self_type& rubbish::binary_tree_base<T,node>::postorder_iterator::operator++(){
    if(m_cursor==nullptr||m_cursor->empty()){
        m_cursor.reset();
        return *this;
    }
    auto ptr=m_cursor->back();m_cursor->pop_back();
    if(m_cursor->empty()){
        m_cursor.reset();
        return *this;
    }
    auto stack_top = m_cursor->back(), tmp = stack_top->right;
    if(ptr == stack_top->left && tmp != nullptr) {
        for(;;) {
            m_cursor->push_back(tmp);
            if(tmp->left != nullptr)
                tmp = tmp->left;
            else if(tmp->right != nullptr)
                tmp = tmp->right;
            else 
                break;
        }
    }
    // Double check if the iteration comes to an end
    if(m_cursor->empty())
        m_cursor.reset();
    return *this;
}

template <class T,class node> rubbish::binary_tree_base<T,node>::level_iterator::level_iterator(node *root):base_class(root) {
    if(m_cursor!=nullptr)
        m_cursor->push_back(root);
}

template <class T,class node> typename rubbish::binary_tree_base<T,node>::level_iterator::self_type& rubbish::binary_tree_base<T,node>::level_iterator::operator++() {
    if(m_cursor==nullptr||m_cursor->empty()){
        m_cursor.reset();
        return *this;
    }
    auto ptr=m_cursor->front();m_cursor->pop_front();
    if(ptr->left != nullptr)
        m_cursor->push_back(ptr->left);
    if(ptr->right != nullptr)
        m_cursor->push_back(ptr->right);
    // Double check if the iteration comes to an end
    if(m_cursor->empty())
        m_cursor.reset();
    return *this;
}
