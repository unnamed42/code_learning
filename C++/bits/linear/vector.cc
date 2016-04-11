template <class T> void rubbish::vector<T>::clear() {
    for(auto i=0UL;i<m_used;++i)
        reinterpret_cast<T*>(m_base+i)->~T();
    m_used=0;
}

template <class T> void rubbish::vector<T>::push_back(const T &value){
    if(m_used+1>m_size)
        resize();
    m_base[m_used++]=value;
}

template <class T> void rubbish::vector<T>::pop_back() {
    if(m_used==0)
        return;
    reinterpret_cast<T*>(m_base+(--m_used))->~T();
}

template <class T> void rubbish::vector<T>::resize() {
    if(m_size==0)
        m_size+=rubbish::__vec_base;
    else
        m_size<<=1;
    auto *tmp=new T[m_size];
    for(auto i=0UL;i<m_used;++i)
        tmp[i]=m_base[i];
    delete[] m_base;
    m_base=tmp;
}

template <class T> rubbish::vector<T>& rubbish::vector<T>::operator=(const rubbish::vector<T> &other){
    delete[] m_base;
    m_size=other.m_size;
    m_used=other.m_used;
    m_base=new T[other.m_size];
    for(auto i=0UL;i<m_used;++i)
        m_base[i]=other[i];
    return *this;
}

template <class T> rubbish::vector<T>& rubbish::vector<T>::operator=(rubbish::vector<T> &&other){
    delete[] m_base;
    m_size=other.m_size;
    m_used=other.m_used;
    m_base=other.m_base;
    other.m_base=nullptr;
}
