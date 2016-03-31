// Binary tree base class function implementions
template <class T,class Node> rubbish::binary_tree_base<T,Node>::binary_tree_base(node *root): m_root(root) {}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::binary_tree_base(const binary_tree_base<T,Node> &tree) {copy_subtree(m_root,tree.m_root);}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::binary_tree_base(binary_tree_base<T,Node> &&tree):m_root(tree.m_root) {tree.m_root = nullptr;}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::binary_tree_base(std::initializer_list<T> &&pre,std::initializer_list<T> &&in):m_root(nullptr) {
    // Algorithm from https://leetcode.com/discuss/28271/my-o-n-19ms-solution-without-recusion-hope-help-you
    if(pre.size()==0||in.size()==0||pre.size()!=in.size())
        return;
    std::deque<node*> stack;
    auto ipre=pre.begin();      // iterator of `pre`
    m_root = new node(*(ipre++));
    stack.push_back(m_root); 
    auto iin=in.begin();        // iterator of `in`
    for(;ipre!=pre.end();++ipre) {
        // Keep pushing the nodes from the preorder into a stack,
        // and keep making the tree by adding nodes to the left of the previous node
        // until the top of the stack matches the inorder.
        node *tmp= stack.back();
        if((tmp->data)!=*iin) {
            tmp->left = new node(*ipre);
            tmp->left->parent=tmp;
            stack.push_back(tmp->left);
        } else {
            // At this point, pop the top of the stack until the top does not equal inorder 
            while(!stack.empty() && (stack.back()->data)==*iin) {
                tmp=stack.back(); stack.pop_back(); 
                ++iin; 
            }
            // TODO: change the result of difference to some unsigned type
            if((iin-in.begin())< in.size()) {
                tmp->right = new node(*ipre);
                tmp->right->parent=tmp;
                stack.push_back(tmp->right);
            }
        }
    }
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::binary_tree_base(std::initializer_list<T> &&v, const T &null): m_root(nullptr) {
    auto size = v.size();
    auto it = v.begin();
    if(size == 0 || *it == null)
        return;
    std::deque<node*> queue;
    m_root = new node(*(it++));
    queue.push_back(m_root);
    bool is_left = true;
    node *cur = nullptr;
    for(; it != v.end(); ++it) {
        node *ptr = nullptr;
        if(*it != null) {
            ptr = new node(*it);
            queue.push_back(ptr);
        }
        if(is_left) {
            cur = queue.front();
            queue.pop_front();
            cur->left = ptr;
            if(ptr!=nullptr)
                ptr->parent=cur;
            is_left = false;
        } else {
            cur->right = ptr;
            if(ptr!=nullptr)
                ptr->parent=cur;
            is_left = true;
        }
    }
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::~binary_tree_base() {delete_subtree(m_root);}

template <class T,class Node> void rubbish::binary_tree_base<T,Node>::delete_subtree(const node *root) {
    if(root == nullptr)
        return;
    delete_subtree(root->left);
    delete_subtree(root->right);
    delete root;
}

template <class T,class Node> void rubbish::binary_tree_base<T,Node>::copy_subtree(node* &dest, const node *src) {
    if(src == nullptr){
        dest=nullptr;
        return;
    }
    if(dest != nullptr)
        delete_subtree(dest);
    dest = new node(*src);
    copy_subtree(dest->left, src->left);
    copy_subtree(dest->right, src->right);
    if(dest->left!=nullptr)
        dest->left->parent=dest;
    if(dest->right!=nullptr)
        dest->right->parent=dest;
}

template <class T,class Node> void rubbish::binary_tree_base<T,Node>::insert_parent(const T &_data, CHILD LR) {
    node *parent = new node(_data);
    if(LR == LEFT)
        parent->left = m_root;
    else
        parent->right = m_root;
    m_root->parent=parent;
    m_root = parent;
}

template <class T,class Node> void rubbish::binary_tree_base<T,Node>::insert_child(const T &_data, CHILD LR) {
    node *child = new node(_data);
    if(m_root == nullptr) {
        m_root = child;
        return;
    }
    child->parent=m_root;
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

template <class T,class Node> unsigned int rubbish::binary_tree_base<T,Node>::depth() const {return depth(m_root);}

template <class T,class Node> unsigned int rubbish::binary_tree_base<T,Node>::depth(const node *root) const {
    if(root == nullptr)
        return 0;
    unsigned int left = depth(root->left), right = depth(root->right);
    return 1 + (left > right ? left : right);
}

template <class T,class Node> void rubbish::binary_tree_base<T,Node>::clear() {
    delete_subtree(m_root);
    m_root=nullptr;
}

template <class T,class Node> bool rubbish::binary_tree_base<T,Node>::empty() const noexcept {return m_root==nullptr;}


// Iterator function implementions

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::preorder_iterator::self_type& rubbish::binary_tree_base<T,Node>::preorder_iterator::operator++(){
    if(m_cursor==nullptr)
        return *this;
    if(m_cursor->left!=nullptr)
        m_cursor=m_cursor->left;
    else if(m_cursor->right!=nullptr)
        m_cursor=m_cursor->right;
    else{
        data_type parent=m_cursor->parent;
        while(parent!=nullptr){
            // If right-subtree is done or empty, then the whole tree is done, traceback.
            if(parent->right==nullptr || m_cursor==parent->right){
                m_cursor=parent;
                parent=parent->parent;
                continue;
            }
            // If left-subtree is done, the next will be right-subtree.
            if(m_cursor==parent->left){
                m_cursor=parent->right;
                return *this;
            }
        }
        // Whole tree is done.
        m_cursor=nullptr;
    }
    return *this;
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::inorder_iterator::inorder_iterator(const data_type &root):base_class(root) {
    if(m_cursor==nullptr)
        return;
    while(m_cursor->left!=nullptr)
        m_cursor=m_cursor->left;
}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::inorder_iterator::self_type& rubbish::binary_tree_base<T,Node>::inorder_iterator::operator++(){
    if(m_cursor==nullptr)
        return *this;
    // When we take a tree root, we can believe that its left-subtree is done.
    if(m_cursor->right!=nullptr){
        m_cursor=m_cursor->right;
        while(m_cursor->left!=nullptr)
            m_cursor=m_cursor->left;
    } else {
        // If one tree's right-subtree is also done,
        // then traceback until it's "come from left".
        data_type parent=m_cursor->parent;
        while(parent!=nullptr){
            if(m_cursor==parent->left){
                m_cursor=parent;
                return *this;
            }
            m_cursor=parent;
            parent=parent->parent;
        }
        m_cursor=nullptr;
    }
    return *this;
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::postorder_iterator::postorder_iterator(const data_type &root):base_class(root) {
    if(m_cursor==nullptr)
        return;
    for(;;){
        if(m_cursor->left!=nullptr)
            m_cursor=m_cursor->left;
        else if(m_cursor->right!=nullptr)
            m_cursor=m_cursor->right;
        else
            break;
    }
}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::postorder_iterator::self_type& rubbish::binary_tree_base<T,Node>::postorder_iterator::operator++(){
    if(m_cursor==nullptr)
        return *this;
    // When we take a node, we can believe that all of its subtrees are done
    node *parent=m_cursor->parent;
    if(parent!=nullptr){
        // If parent's left-subtree is done and right-subtree is undone, go to right
        if(m_cursor==parent->left && parent->right!=nullptr){
            m_cursor=parent->right;
            for(;;){
                if(m_cursor->left!=nullptr)
                    m_cursor=m_cursor->left;
                else if(m_cursor->right!=nullptr)
                    m_cursor=m_cursor->right;
                else
                    break;
            }
        }else 
            // Else, all done, traceback
            // In this branch, m_cursor==parent->right
            m_cursor=parent;
    } else 
        // Whole tree is done
        m_cursor=nullptr;
    return *this;
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>::level_iterator::level_iterator(const data_type &root):m_cursor(nullptr) {
    if(root!=nullptr){
        m_cursor=std::make_shared<std::deque<data_type>>(std::deque<data_type>());
        m_cursor->push_back(root);
        // Make iteration doable starting at any node
        data_type parent=root->parent;
        if(parent!=nullptr && parent->left==root)
            m_cursor->push_back(parent->right);
    }
}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::level_iterator::self_type& rubbish::binary_tree_base<T,Node>::level_iterator::operator++() {
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

template <class T,class Node> bool rubbish::binary_tree_base<T,Node>::level_iterator::operator==(const self_type &other) const {
    if(m_cursor==other.m_cursor)
        return true;
    return m_cursor!=nullptr && other.m_cursor!=nullptr && m_cursor->front()==other.m_cursor->front();
}
