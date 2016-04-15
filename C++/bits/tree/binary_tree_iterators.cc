// This file is included inside a class declaration,
// so do not do declarations or include files here

            class tree_iterator_base:public iterator<std::forward_iterator_tag,T> {
                private:
                    typedef iterator<std::forward_iterator_tag,T> base_class;
                public:
                    typedef typename base_class::reference reference;
                    typedef typename base_class::pointer   pointer;
                    
                    typedef tree_iterator_base self_type;
                    typedef node*              data_type;
                    
                    explicit tree_iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    tree_iterator_base(const tree_iterator_base &other):m_cursor(const_cast<decltype(m_cursor)>(other.get())) {}
                    virtual ~tree_iterator_base() {}
                    
                    reference operator*() const {return m_cursor->data;}
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor;}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const tree_iterator_base &other) {m_cursor=const_cast<decltype(m_cursor)>(other.get()); return *this;}
                protected:
                    data_type m_cursor;
            };
            
            class preorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef preorder_iterator          self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit preorder_iterator(const data_type &cursor):base_class(cursor) {}
                    preorder_iterator(const preorder_iterator &other):base_class(other) {}
                    
                    self_type& operator++(){
                        if(m_cursor==nullptr)
                            return *this;
                        if(m_cursor->left!=nullptr)
                            m_cursor=m_cursor->left;
                        else if(m_cursor->right!=nullptr)
                            m_cursor=m_cursor->right;
                        else{
                            auto parent=m_cursor->parent;
                            while(parent!=nullptr){
                                // If right-subtree is done or empty, then the whole tree is done, traceback.
                                if(parent->right==nullptr || m_cursor==parent->right){
                                    m_cursor=parent;
                                    parent=parent->parent;
                                    continue;
                                }
                                // If left-subtree is done, the next will be right-subtree.
                                if(m_cursor==parent->left){
                                    m_cursor=parent->right;
                                    return *this;
                                }
                            }
                            // Whole tree is done.
                            m_cursor=nullptr;
                        }
                        return *this;
                    }
                    
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            class inorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef inorder_iterator   self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit inorder_iterator(const data_type &cursor):base_class(cursor) {
                        if(m_cursor==nullptr)
                            return;
                        while(m_cursor->left!=nullptr)
                            m_cursor=m_cursor->left;
                    }
                    inorder_iterator(const inorder_iterator &other):base_class(other) {}
                    self_type& operator++(){
                        if(m_cursor==nullptr)
                            return *this;
                        // When we take a tree root, we can believe that its left-subtree is done.
                        if(m_cursor->right!=nullptr){
                            m_cursor=m_cursor->right;
                            while(m_cursor->left!=nullptr)
                                m_cursor=m_cursor->left;
                        } else {
                            // If one tree's right-subtree is also done,
                            // then traceback until it's "come from left".
                            auto parent=m_cursor->parent;
                            while(parent!=nullptr){
                                if(m_cursor==parent->left){
                                    m_cursor=parent;
                                    return *this;
                                }
                                m_cursor=parent;
                                parent=parent->parent;
                            }
                            m_cursor=nullptr;
                        }
                        return *this;
                    }
                    
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            class postorder_iterator: public tree_iterator_base{
                private:
                    typedef tree_iterator_base base_class;
                public:
                    typedef postorder_iterator         self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit postorder_iterator(const data_type &cursor):base_class(cursor) {
                        if(m_cursor==nullptr)
                            return;
                        for(;;){
                            if(m_cursor->left!=nullptr)
                                m_cursor=m_cursor->left;
                            else if(m_cursor->right!=nullptr)
                                m_cursor=m_cursor->right;
                            else
                                break;
                        }
                    }
                    postorder_iterator(const postorder_iterator &other):base_class(other) {}
                    self_type& operator++(){
                        if(m_cursor==nullptr)
                            return *this;
                        // When we take a node, we can believe that all of its subtrees are done
                        node *parent=m_cursor->parent;
                        if(parent!=nullptr){
                            // If parent's left-subtree is done and right-subtree is undone, go to right
                            if(m_cursor==parent->left && parent->right!=nullptr){
                                m_cursor=parent->right;
                                for(;;){
                                    if(m_cursor->left!=nullptr)
                                        m_cursor=m_cursor->left;
                                    else if(m_cursor->right!=nullptr)
                                        m_cursor=m_cursor->right;
                                    else
                                        break;
                                }
                            }else 
                                // Else, all done, traceback
                                // In this branch, m_cursor==parent->right
                                m_cursor=parent;
                        } else 
                            // Whole tree is done
                            m_cursor=nullptr;
                        return *this;
                    }
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                private:
                    using base_class::m_cursor;
            };
            
            // This kind of iterator is totally different from the above three kinds,
            // so do not inherit from tree_iterator_base here
            class level_iterator:public iterator<std::forward_iterator_tag,T> {
                private:
                    typedef iterator<std::forward_iterator_tag,T> base_class;
                public:
                    typedef typename base_class::reference reference;
                    typedef typename base_class::pointer   pointer;
                    
                    typedef level_iterator self_type;
                    typedef node*                           data_type;
                    
                    explicit level_iterator(const data_type &cursor):m_cursor() {
                        if(cursor!=nullptr){
                            m_cursor.push_back(cursor);
                            // Make iteration doable starting at any node
                            auto parent=cursor->parent;
                            if(parent!=nullptr && parent->left==cursor)
                                m_cursor.push_back(parent->right);
                        }
                    }
                    level_iterator(const level_iterator &other):m_cursor(const_cast<decltype(m_cursor)&>(other.get())) {}
                    level_iterator(level_iterator &&other):m_cursor(std::move(const_cast<decltype(m_cursor)&&>(other.get()))) {}
                    ~level_iterator() = default;
                    
                    reference operator*() const {return const_cast<reference>(m_cursor.front()->data);}
                    pointer operator->() const { return &operator*(); }
                    const std::deque<data_type>& get() const {return m_cursor;}
                    self_type& operator++() {
                        if(m_cursor.empty())
                            return *this;
                        auto ptr=m_cursor.front();m_cursor.pop_front();
                        if(ptr->left != nullptr)
                            m_cursor.push_back(ptr->left);
                        if(ptr->right != nullptr)
                            m_cursor.push_back(ptr->right);
                        return *this;
                    }
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const level_iterator &other) const {return (m_cursor.empty()&&other.m_cursor.empty()) || (!m_cursor.empty()&&!other.m_cursor.empty() && m_cursor.front()==other.m_cursor.front());}
                    bool operator!=(const level_iterator &other) const {return !operator==(other);}
                    self_type& operator=(const level_iterator &other) {m_cursor=const_cast<decltype(m_cursor)&>(other.get()); return *this;}
                    self_type& operator=(level_iterator &&other) {m_cursor=std::move(const_cast<decltype(m_cursor)&>(other.get())); return *this;}
                private:
                    std::deque<data_type> m_cursor;
            };
