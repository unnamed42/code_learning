
template <class T> rubbish::list_base<T>::list_base(std::size_t len,const T &value):m_head(nullptr),m_end(nullptr),m_length(len){
    if(len==0)
        return;
    node dummy;
    node *ptr=&dummy;
    for(;len>0;--len){
        ptr->next=new node(value);
        ptr->next->prev=ptr;
        ptr=ptr->next;
    }
    m_end=ptr;m_head=dummy.next;
    m_head->prev=nullptr;
}

template <class T> rubbish::list_base<T>::list_base(rubbish::list_base<T> &&other){
    m_head=other.m_head;
    m_end=other.m_end;
    m_length=other.m_length;
    other.m_head=other.m_end=nullptr;
}

template <class T> rubbish::list_base<T>::list_base(std::initializer_list<T> &&l):m_head(nullptr),m_end(nullptr),m_length(0){
    if(l.size()==0)
        return;
    auto it=l.begin();
    m_head=m_end=new node(*(it++));
    for(;it!=l.end();++it){
        m_end->next=new node(*it);
        m_end->next->prev=m_end;
        m_end=m_end->next;
    }
}

template <class T> rubbish::list_base<T>::list_base(const rubbish::list_base<T> &l){
    node *another=l.m_head;
    m_head=m_end=new node(another);
    another=another->next;
    while(another!=nullptr){
        m_end->next=new node(another);
        m_end->next->prev=m_end;
        m_end=m_end->next;
        another=another->next;
    }
}

template <class T> rubbish::list_base<T>::~list_base(){
    node *save;
    while(m_head!=nullptr){
        save=m_head->next;
        delete m_head;
        m_head=save;
    }
}

template <class T> std::size_t rubbish::list_base<T>::size() const noexcept {return m_length;}

template <class T> void rubbish::list_base<T>::reverse(){
    if(m_head==nullptr||m_head->next==nullptr)
        return;
    m_end=m_head;
    node *_node=m_head->next;
    while(_node->next!=nullptr){
        node *temp=_node->prev;
        _node->prev=_node->next;
        _node->next=temp;
        _node=_node->prev;
    }
    m_head=_node;
}

template <class T> void rubbish::list_base<T>::push_back(const T &elem){
    ++m_length;
    if(m_head==nullptr){
        m_head=m_end=new node(elem);
        return;
    }
    m_end->next=new node(elem);
    m_end->next->prev=m_end;
    m_end=m_end->next;
}

template <class T> void rubbish::list_base<T>::pop_back(){
    if(m_end==nullptr)
        throw std::out_of_range("list_base::pop_back: empty list!");
    --m_length;
    if(m_end==m_head){
        delete m_end;
        m_head=m_end=nullptr;
        return;
    }
    node *save=m_end->prev;
    save->next=nullptr;
    delete m_end;
    m_end=save;
}

template <class T> void rubbish::list_base<T>::pop_front(){
    if(m_head==nullptr)
        throw std::out_of_range("list_base::pop_back: empty list!");
    --m_length;
    if(m_end==m_head){
        delete m_end;
        m_head=m_end=nullptr;
        return;
    }
    node *save=m_head->next;
    save->prev=nullptr;
    delete m_head;
    m_head=save;
}

template <class T> void rubbish::list_base<T>::push_front(const T &elem){
    ++m_length;
    if(m_head==nullptr){
        m_head=m_end=new node(elem);
        return;
    }
    node *tmp=new node(elem);
    tmp->next=m_head;
    m_head->prev=tmp;
    m_head=tmp;
}

template <class T> void rubbish::list_base<T>::remove(iterator it){
    node *ptr=it.get();
    if(ptr==nullptr)
        return;
    --m_length;
    if(ptr==m_head && ptr==m_end){
        delete ptr;
        m_head=m_end=nullptr;
    } else if(ptr==m_head) {
        ptr=m_head->next;
        delete m_head;
        m_head=ptr;
    } else if(ptr==m_end) {
        ptr=m_end->prev;
        delete m_end;
        m_end=ptr;
    } else {
        ptr->next->prev=ptr->prev;
        ptr->prev->next=ptr->next;
        delete ptr;
    }
}

template <class T> typename rubbish::list_base<T>::iterator rubbish::list_base<T>::begin() {return iterator(m_head);}

template <class T> typename rubbish::list_base<T>::iterator rubbish::list_base<T>::end() {return iterator(nullptr);}

template <class T> typename rubbish::list_base<T>::reverse_iterator rubbish::list_base<T>::rbegin() {return reverse_iterator(m_end);}

template <class T> typename rubbish::list_base<T>::reverse_iterator rubbish::list_base<T>::rend() {return reverse_iterator(nullptr);}


