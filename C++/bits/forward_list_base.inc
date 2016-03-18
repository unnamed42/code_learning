// member functions
template <class T> rubbish::forward_list_base<T>::forward_list_base(std::size_t len,const T &value):m_head(nullptr),m_end(nullptr),m_length(len){
    if(len==0)
        return;
    m_end=m_head=new node(value);
    for(;len>1;len--){
        m_end->next=new node(value);
        m_end=m_end->next;
    }
}

template <class T> rubbish::forward_list_base<T>::forward_list_base(rubbish::forward_list_base<T> &&other){
    m_head=other.m_head;
    m_end=other.m_end;
    m_length=other.m_length;
    other.m_head=other.m_end=nullptr;
}

template <class T> rubbish::forward_list_base<T>::forward_list_base(const rubbish::forward_list_base<T> &other):m_head(nullptr),m_end(nullptr),m_length(other.m_length){
    node **ptr=&m_head;const node *optr=other.m_head;
    while(optr!=nullptr){
        *ptr=new node(optr);
        ptr=&(*ptr)->next;
    }
}

template <class T> rubbish::forward_list_base<T>::forward_list_base(std::initializer_list<T> &&l):m_head(nullptr),m_end(nullptr),m_length(0){
    if(l.size()==0)
        return;
    auto it=l.begin();
    m_head=m_end=new node(*(it++));
    for(;it!=l.end();++it){
        m_end->next=new node(*it);
        m_end=m_end->next;
    }
}

template <class T> rubbish::forward_list_base<T>::~forward_list_base(){
    while(m_head!=nullptr){
        m_end=m_head->next;
        delete m_head;
        m_head=m_end;
    }
}

template <class T> void rubbish::forward_list_base<T>::remove_elem(const T &elem){
    if(m_head->data==elem){
        if(m_head->next==nullptr){
            delete m_head;
            m_length--;
            m_head=nullptr;
            return;
        }
        node *ptr=m_head->next;
        delete m_head;
        m_length--;
        m_head=ptr;
        this->remove_elem(elem);
    }
    node *ptr=m_head->next,*prev=m_head;
    while(ptr!=nullptr){
        if(ptr->elem==elem){
            prev->next=ptr->next;
            delete ptr;
            m_length--;
            ptr=prev->next;
        } else {
            prev=ptr;
            ptr=ptr->next;
        }
    }
    if(prev->next==nullptr)
        m_end=prev;
}

template <class T> std::size_t rubbish::forward_list_base<T>::size() const noexcept {return m_length;}

template <class T> typename rubbish::forward_list_base<T>::iterator rubbish::forward_list_base<T>::search(const T &elem) const{
    node *p=m_head;
    while(p!=nullptr){
        if(p->data==elem)
            return iterator(p);
        p=p->next;
    }
    return iterator(nullptr);
}

template <class T> void rubbish::forward_list_base<T>::insert_after(iterator it,const T &elem){
    node const *_it=it.get();
    if(_it==nullptr)
        return;
    if(_it==m_end){
        m_end->next=new node(elem);
        m_end=m_end->next;
    } else {
        node *temp=new node(elem);
        temp->next=_it->next;
        _it->next=temp;
    }
}

template <class T> void rubbish::forward_list_base<T>::push_back(const T &elem){
    ++m_length;
    if(m_head==nullptr){
        m_head=m_end=new node(elem);
        return;
    }
    m_end->next=new node(elem);
    m_end=m_end->next;
}

template <class T> void rubbish::forward_list_base<T>::pop_front(){
    if(m_head==nullptr)
        throw std::out_of_range("forward_list_base::pop_front: empty list!");
    --m_length;
    if(m_end==m_head){
        delete m_end;
        m_head=m_end=nullptr;
        return;
    }
    node *save=m_head->next;
    delete m_head;
    m_head=save;
}

template <class T> void rubbish::forward_list_base<T>::push_front(const T &elem){
    ++m_length;
    if(m_head==nullptr){
        m_head=m_end=new node(elem);
        return;
    }
    node *tmp=new node(elem);
    tmp->next=m_head;
    m_head=tmp;
}

template <class T> T& rubbish::forward_list_base<T>::back() const {return m_end->data;}

template <class T> typename rubbish::forward_list_base<T>::iterator rubbish::forward_list_base<T>::begin(){ return iterator(m_head); }

template <class T> typename rubbish::forward_list_base<T>::iterator rubbish::forward_list_base<T>::end(){ return iterator(nullptr); }

template <class T> void rubbish::forward_list_base<T>::reverse(){
    if(m_head==nullptr||m_head->next==nullptr)
        return;
    m_end=m_head;
    node *nhead=nullptr;
    while(m_head!=nullptr){
        node *p=m_head->next;
        m_head->next=nhead;
        nhead=m_head;
        m_head=p;
    }
    m_head=nhead;
}

template <class T> void rubbish::forward_list_base<T>::remove_after(iterator it){
    node const *_it=it.get();
    if(_it==nullptr||_it->next==nullptr)
        return;
    node *save=_it->next->next;
    delete _it->next;
    _it->next=save;
    m_length--;
}
