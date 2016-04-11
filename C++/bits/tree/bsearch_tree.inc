template <class T,class Node> rubbish::bsearch_tree<T,Node>::bsearch_tree(node *root):base_class(root) {}

template <class T,class Node> rubbish::bsearch_tree<T,Node>::bsearch_tree(std::initializer_list<T> &&l){
    for(auto &&i:l)
        insert(m_root,i);
}

template <class T,class Node> void rubbish::bsearch_tree<T,Node>::insert(node* &root,const T &data){
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

template <class T,class Node> void rubbish::bsearch_tree<T,Node>::insert(const T &data) {insert(m_root,data);}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::begin() {return iterator(m_root);}

template <class T,class Node> typename rubbish::bsearch_tree<T,Node>::iterator rubbish::bsearch_tree<T,Node>::end() {return iterator(nullptr);}
