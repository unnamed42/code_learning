// helper function
template <class T> static inline T max(const T &lhs,const T &rhs) { return lhs>rhs?lhs:rhs; }

template <class T> static inline short height(const rubbish::avl_tree_node<T> *p) { return p==nullptr?0:p->height; }

template <class T> static inline short balance(const rubbish::avl_tree_node<T> *p) { return p==nullptr?0:height(p->left)-height(p->right); }

// return the node with minimum value found in a tree
template <class node> static inline node* min_node(node *root){
    auto current = root;
    /* loop down to find the leftmost leaf */
    while (current->left != nullptr)
        current = current->left;
 
    return current;
}


template <class T,class node> rubbish::avl_tree<T,node>::avl_tree():base_class() {}

template <class T,class node> rubbish::avl_tree<T,node>::avl_tree(std::initializer_list<T> &&l):base_class() {
    for(auto &&i:l)
        insert(i);
}

template <class T,class node> const node* rubbish::avl_tree<T,node>::find(const T &value) const {
    auto ptr=m_root;
    while(ptr!=nullptr){
        if(ptr->data==value)
            break;
        else if(ptr->data>value)
            ptr=ptr->left;
        else
            ptr=ptr->right;
    }
    return ptr;
}


template <class T,class node> const node* rubbish::avl_tree<T,node>::left_rotate(node* &root){
    auto x=root->right,y=x->left;
    
    // Rotation
    x->left = root;
    root->left = y;
    
    // Update height
    root->height = max(height(root->left),height(root->right))+1;
    x->height = max(height(x->left),height(x->right))+1;
    
    // Return new tree root
    return root=x;
}

// Right rotate subtree rooted with y
template <class T,class node> const node* rubbish::avl_tree<T,node>::right_rotate(node* &root){
    auto x=root->left,y=root->right;
    
    // Rotation
    x->right = root;
    root->left = y;
    
    // Update height
    root->height = max(height(root->left),height(root->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
    
    // Return new root
    return root=x;
}
 
template <class T,class node> const node* rubbish::avl_tree<T,node>::insert(node* &root,const T &value){
    // Perform the normal BST rotation
    if(root==nullptr){
        root=new node(value);
        return root;
    }
    
    if (value < root->data)
        return insert(root->left, value);
    else if(value == root->data)
        return root;
    else 
        return insert(root->right, value);
    
    // Update root height
    root->height = max(height(root->left), height(root->right)) + 1;
    
    // Get balance factor of root
    auto factor = balance(root);
    
    // When unbalanced, do some rotations
    // 1. Left Left Case
    if (factor > 1 && value < root->left->data)
        return right_rotate(root);
    
    // 2. Right Right Case
    if (factor < -1 && value > root->right->data)
        return left_rotate(root);
    
    // 3. Left Right Case
    if (factor > 1 && value > root->left->data){
        left_rotate(root->left);
        return right_rotate(root);
    }
    
    // 4. Right Left Case
    if (factor < -1 && value < root->right->data){
        right_rotate(root->right);
        return left_rotate(root);
    }
    
    // 5. A balanced root, do nothing
    return root;
}

template <class T,class node> void rubbish::avl_tree<T,node>::erase(node* &root,const T &value){
    // PERFORM STANDARD BST DELETE
    if (root == nullptr)
        return;
    
    // If the value to be deleted is smaller than the root's value,
    // then it lies in left subtree
    if ( value < root->data )
        erase(root->left, value);
    
    // If the value to be deleted is greater than the root's value,
    // then it lies in right subtree
    else if( value > root->data )
        erase(root->right, value);
    
    // if value is same as root's value, then This is the node
    // to be deleted
    else{
        // node with only one child or no child
        if( (root->left == nullptr) || (root->right == nullptr) ){
             auto temp = root->left!=nullptr ? root->left : root->right;
 
            // No child case
            if(temp == nullptr){
                temp = root;
                root = nullptr;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child
    
            delete temp;
        } else {
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            auto temp = min_node(root->right);
    
            // Copy the inorder successor's data to this node
            root->data = temp->data;
    
            // Delete the inorder successor
            erase(root->right, temp->data);
        }
    }
    
    // If the tree had only one node then return
    if (root == nullptr)
      return;
    
    // UPDATE HEIGHT OF THE CURRENT NODE
    root->height = max(height(root->left), height(root->right)) + 1;
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
    //  this node became unbalanced)
    auto factor = balance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (factor > 1 && balance(root->left) >= 0){
        right_rotate(root);
        return;
    }
 
    // Left Right Case
    if (factor > 1 && balance(root->left) < 0){
        left_rotate(root->left);
        right_rotate(root);
        return;
    }
 
    // Right Right Case
    if (factor < -1 && balance(root->right) <= 0){
        left_rotate(root);
        return;
    }
 
    // Right Left Case
    if (factor < -1 && balance(root->right) > 0){
        right_rotate(root->right);
        left_rotate(root);
        return;
    }
}

template <class T,class node> const node* rubbish::avl_tree<T,node>::insert(const T &value) { return insert(m_root,value); }

template <class T,class node> void rubbish::avl_tree<T,node>::erase(const T &value) { return erase(m_root,value); }

template <class T,class node> typename rubbish::avl_tree<T,node>::iterator rubbish::avl_tree<T,node>::begin() { return iterator(m_root);}

template <class T,class node> typename rubbish::avl_tree<T,node>::iterator rubbish::avl_tree<T,node>::end() { return iterator(nullptr);}
