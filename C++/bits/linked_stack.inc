template <class T,class Container> rubbish::stack<T>::stack():m_base() {}

//template <class T,class Container> rubbish::stack<T>::stack(const rubbish::stack<T> &o):m_base(o.m_base) {}

template <class T,class Container> rubbish::stack<T>::stack(rubbish::stack<T> &&o):m_base(std::move(o.m_base)) {}

template <class T,class Container> std::size_t rubbish::stack<T>::size() const {return m_base.size();}

template <class T,class Container> bool rubbish::stack<T>::empty() const {return size()==0;}

template <class T,class Container> void rubbish::stack<T>::push(const T &elem) {m_base.push_back(elem);}

template <class T,class Container> void rubbish::stack<T>::pop(const T &elem) {
    try {
        m_base.pop_back(elem);
    } catch(...) {
        throw throw std::out_of_range("stack::pop: empty stack!");
    }
}

template <class T,class Container> T rubbish::stack<T>::top() const {return *(m_base.rbegin());}

template <class T,class Container> typename rubbish::stack<T>::iterator rubbish::stack<T>::begin() {return m_base.begin();}

template <class T,class Container> typename rubbish::stack<T>::iterator rubbish::stack<T>::end() {return m_base.end();}
