
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

template <class T,class Node> unsigned int rubbish::binary_tree_base<T,Node>::depth(const node *root) {
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

template <class T,class Node> rubbish::binary_tree_base<T,Node>& rubbish::binary_tree_base<T,Node>::operator=(const rubbish::binary_tree_base<T,Node> &o) {
    this->~binary_tree_base();
    m_root=nullptr;
    copy_subtree(m_root,o.m_root);
    return *this;
}

template <class T,class Node> rubbish::binary_tree_base<T,Node>& rubbish::binary_tree_base<T,Node>::operator=(rubbish::binary_tree_base<T,Node> &&o) {
    this->~binary_tree_base();
    m_root=o.m_root;
    o.m_root=nullptr;
}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::preorder_iterator rubbish::binary_tree_base<T,Node>::preorder_begin() {return preorder_iterator(m_root);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::preorder_iterator rubbish::binary_tree_base<T,Node>::preorder_end() {return preorder_iterator(nullptr);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::inorder_iterator rubbish::binary_tree_base<T,Node>::inorder_begin() {return (m_root);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::inorder_iterator rubbish::binary_tree_base<T,Node>::inorder_end() {return (nullptr);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::postorder_iterator rubbish::binary_tree_base<T,Node>::postorder_begin() {return (m_root);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::postorder_iterator rubbish::binary_tree_base<T,Node>::postorder_end() {return (nullptr);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::level_iterator rubbish::binary_tree_base<T,Node>::level_begin() {return level_iterator(m_root);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::level_iterator rubbish::binary_tree_base<T,Node>::level_end() {return level_iterator(nullptr);}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_preorder_iterator rubbish::binary_tree_base<T,Node>::preorder_cbegin() const {return const_preorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->preorder_begin());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_preorder_iterator rubbish::binary_tree_base<T,Node>::preorder_cend() const {return const_preorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->preorder_end());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_inorder_iterator rubbish::binary_tree_base<T,Node>::inorder_cbegin() const {return const_inorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->inorder_begin());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_inorder_iterator rubbish::binary_tree_base<T,Node>::inorder_cend() const {return const_inorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->inorder_end());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_postorder_iterator rubbish::binary_tree_base<T,Node>::postorder_cbegin() const {return const_postorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->postorder_begin());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_postorder_iterator rubbish::binary_tree_base<T,Node>::postorder_cend() const {return const_postorder_iterator(const_cast<binary_tree_base<T,Node>*>(this)->postorder_end());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_level_iterator rubbish::binary_tree_base<T,Node>::level_cbegin() const {return const_level_iterator(const_cast<binary_tree_base<T,Node>*>(this)->level_begin());}

template <class T,class Node> typename rubbish::binary_tree_base<T,Node>::const_level_iterator rubbish::binary_tree_base<T,Node>::level_cend() const {return const_level_iterator(const_cast<binary_tree_base<T,Node>*>(this)->level_end());}
