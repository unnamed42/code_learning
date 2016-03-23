template <class T,class Container> rubbish::stack<T,Container>::stack():m_base() {}

//template <class T,class Container> rubbish::stack<T,Container>::stack(const rubbish::stack<T,Container> &o):m_base(o.m_base) {}

template <class T,class Container> rubbish::stack<T,Container>::stack(rubbish::stack<T,Container> &&o):m_base(std::move(o.m_base)) {}

template <class T,class Container> std::size_t rubbish::stack<T,Container>::size() const {return m_base.size();}

template <class T,class Container> bool rubbish::stack<T,Container>::empty() const {return size()==0;}

template <class T,class Container> void rubbish::stack<T,Container>::push(const T &elem) {m_base.push_back(elem);}

template <class T,class Container> void rubbish::stack<T,Container>::pop() {
    try {
        m_base.pop_back();
    } catch(...) {
        throw throw std::out_of_range("stack::pop: empty stack!");
    }
}

template <class T,class Container> T rubbish::stack<T,Container>::top() const {return *(m_base.rbegin());}

template <class T,class Container> typename rubbish::stack<T,Container>::iterator rubbish::stack<T,Container>::begin() {return m_base.begin();}

template <class T,class Container> typename rubbish::stack<T,Container>::iterator rubbish::stack<T,Container>::end() {return m_base.end();}
