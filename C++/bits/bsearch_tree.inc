template <class T,class node> rubbish::bsearch_tree<T,node>::bsearch_tree(node *root):base_class(root) {}

template <class T,class node> rubbish::bsearch_tree<T,node>::bsearch_tree(std::initializer_list<T,class node> &&l){
    for(auto &&i:l)
        insert(m_root,i);
}

template <class T,class node> void rubbish::bsearch_tree<T,node>::insert(node* &root,const T &data){
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

template <class T,class node> void rubbish::bsearch_tree<T,node>::insert(const T &data) {insert(m_root,data);}

template <class T,class node> typename rubbish::bsearch_tree<T,node>::iterator rubbish::bsearch_tree<T,node>::begin() {return iterator(m_root);}

template <class T,class node> typename rubbish::bsearch_tree<T,node>::iterator rubbish::bsearch_tree<T,node>::end() {return iterator(nullptr);}
