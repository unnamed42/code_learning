template <class T> rubbish::bsearch_tree<T>::bsearch_tree(std::initializer_list<T> &&l){
    for(auto &&i:l)
        insert(m_root,i);
}

template <class T> void rubbish::bsearch_tree<T>::insert(node* &root,const T &data){
    if(root==nullptr){
        root=new node(data);
        return;
    }
    if(root->data > data)
        insert(root->left,data);
    else if(root->data == data)
        return;
    else
        insert(root->right,data);
}

template <class T> void rubbish::bsearch_tree<T>::insert(const T &data) {insert(m_root,data);}
