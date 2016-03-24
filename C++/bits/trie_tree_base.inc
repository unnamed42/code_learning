template <class Char,class Node> rubbish::trie_tree_base<Char,Node>::trie_tree_base(const Char *str):m_root(NULL_TERMINATOR) {insert(str);}

template <class Char,class Node> void rubbish::trie_tree_base<Char,Node>::insert(const Char *str){
    if(str==nullptr||*str==NULL_TERMINATOR)
        return;
    auto trie_node=&m_root;
    while(*str!=NULL_TERMINATOR){
        auto &&tmp=trie_node->children.insert(node(*str))->data;
        std::cout<<tmp.data<<" ";
        trie_node=&tmp;
        ++str;
    }
    std::cout<<std::endl;
}

template <class Char,class Node> bool rubbish::trie_tree_base<Char,Node>::has_str(const Char *str) const {
    if(str==nullptr)
        return true;
    auto trie_node=&m_root;
    while(*str!=NULL_TERMINATOR){
        auto ptr=trie_node->children.find(node(*str));
        if(ptr==nullptr)
            return false;
        trie_node=&(ptr->data);
        std::cout<<trie_node->data<<" ";
        ++str;
    }
    std::cout<<std::endl;
    return true;
}
