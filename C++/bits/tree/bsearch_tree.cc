template <class T,class Node> rubbish::bsearch_tree<T,Node>::bsearch_tree(node *root):base_class(root) {}

template <class T,class Node> rubbish::bsearch_tree<T,Node>::bsearch_tree(std::initializer_list<T> &&l){
    for(auto &&i:l)
        insert(m_root,i);
}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::node* rubbish::bsearch_tree<T,Node>::insert(node* &root,const T &data){
    if(root==nullptr){
        root=new node(data);
        return root;
    }
    if(root->data > data){
        auto ret=insert(root->left,data);
        root->left->parent=root;
        return ret;
    }
    else if(root->data == data)
        return root;
    else{
        auto ret=insert(root->right,data);
        root->right->parent=root;
        return ret;
    }
}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::insert(const T &data) {return iterator(insert(m_root,data));}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::insert(T &&data) {return iterator(insert(m_root,std::move(data)));}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::const_iterator rubbish::bsearch_tree<T,Node>::find(const T &data) const {
    auto ptr=m_root;
    while(ptr!=nullptr){
        if(ptr->data == data)
            break;
        else if(ptr->data < data)
            ptr=ptr->right;
        else
            ptr=ptr->right;
    }
    return const_iterator(ptr);
}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::find(const T &data) {
    auto ptr=m_root;
    while(ptr!=nullptr){
        if(ptr->data == data)
            break;
        else if(ptr->data < data)
            ptr=ptr->right;
        else
            ptr=ptr->right;
    }
    return iterator(ptr);
}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::begin() {return iterator(m_root);}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::end() {return iterator(nullptr);}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::const_iterator rubbish::bsearch_tree<T,Node>::cbegin() const {return const_iterator(const_cast<bsearch_tree<T,Node>*>(this)->begin());}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::const_iterator rubbish::bsearch_tree<T,Node>::cend() const {return const_iterator(const_cast<bsearch_tree<T,Node>*>(this)->end());}
