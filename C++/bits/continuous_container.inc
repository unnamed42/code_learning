template <class T> rubbish::continuous_container<T>::continuous_container(std::size_t count,const T &value):m_base(nullptr),m_size(count),m_used(count) {
    if(count==0)
        m_base=nullptr;
    m_base=new T[count];
    for(auto i=0UL;i<count;++i)
        m_base[i]=value;
}
