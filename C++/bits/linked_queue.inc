template <class T,class Container> rubbish::queue<T>::queue():m_base() {}

//template <class T,class Container> rubbish::queue<T>::queue(const rubbish::queue<T> &o):m_base(o.m_base) {}

template <class T,class Container> rubbish::queue<T>::queue(rubbish::queue<T> &&o):m_base(std::move(o.m_base)) {}

template <class T,class Container> std::size_t rubbish::queue<T>::size() const {return m_base.size();}

template <class T,class Container> bool rubbish::queue<T>::empty() const {return size()==0;}

template <class T,class Container> void rubbish::queue<T>::push(const T &elem) {m_base.push_back(elem);}

template <class T,class Container> void rubbish::queue<T>::pop(const T &elem) {
    try {
        m_base.pop_front(elem);
    } catch(...) {
        throw throw std::out_of_range("queue::pop: empty queue!");
    }
}

template <class T,class Container> T rubbish::queue<T>::front() const {return *(m_base.begin());}

template <class T,class Container> typename rubbish::queue<T>::iterator rubbish::queue<T>::begin() {return m_base.begin();}

template <class T,class Container> typename rubbish::queue<T>::iterator rubbish::queue<T>::end() {return m_base.end();}
