// Interfaces to make T1 comparable with pair<T1,T2> (such comparasion is used in avl_tree)
template <class T1,class T2> bool operator<(const T1 &lhs,const pair<T1,T2> &rhs){return lhs<rhs.first;}
template <class T1,class T2> bool operator==(const T1 &lhs,const pair<T1,T2> &rhs){return lhs==rhs.first;}
template <class T1,class T2> bool operator>(const T1 &lhs,const pair<T1,T2> &rhs){return lhs>rhs.first;}

template <class T1,class T2> rubbish::map<T1,T2>::map():m_base() {}

template <class T1,class T2> rubbish::map<T1,T2>::map(const map<T1,T2> &o):m_base(o.m_base) {}

template <class T1,class T2> rubbish::map<T1,T2>::map(map<T1,T2> &&o):m_base(std::move(o.m_base)) {}

template <class T1,class T2> rubbish::map<T1,T2>::map(std::initializer_list<value_type>&&l):m_base(std::move(l)) {}

template <class T1,class T2> template <class value> void rubbish::map<T1,T2>::insert(value &&val) {m_base.insert(std::forward(val));}

template <class T1,class T2> template <class key> typename rubbish::map<T1,T2>::value_type* rubbish::map<T1, T2>::find(key &&k) const{
    auto &&map_node=m_base.find(std::forward<T1>(k));
    if(map_node==nullptr)
        return nullptr;
    return &(map_node->data);
}

template <class T1,class T2> template <class key> void rubbish::map<T1, T2>::erase(key &&k){m_base.erase(std::forward<T1>(k));}

template <class T1,class T2> typename rubbish::map<T1, T2>::iterator rubbish::map<T1, T2>::begin() {return m_base.begin();}

template <class T1,class T2> typename rubbish::map<T1, T2>::iterator rubbish::map<T1, T2>::end() {return m_base.end();}
