// This file is included inside a class declaration,
// so do not need to add namespace or include files here

            template <bool not_const> class tree_iterator_base:public iterator<std::forward_iterator_tag,typename condition<not_const,T,const T>::type> {
                private:
                    typedef iterator<std::forward_iterator_tag,typename condition<not_const,T,const T>::type> base_class;
                public:
                    typedef typename base_class::reference reference;
                    typedef typename base_class::pointer pointer;
                    
                    typedef tree_iterator_base<not_const>                         self_type;
                    typedef typename condition<not_const,node*,const node*>::type data_type;
                    
                    explicit tree_iterator_base(const data_type &cursor):m_cursor(cursor) {}
                    tree_iterator_base(const self_type &other):m_cursor(other.m_cursor) {}
                    virtual ~tree_iterator_base() = default;
                    reference operator*() const {return m_cursor->data;}
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor;}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor;}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                protected:
                    data_type m_cursor;
            };
            
            template <bool not_const> class pre_order_iterator: public tree_iterator_base<not_const>{
                private:
                    typedef tree_iterator_base<not_const> base_class;
                public:
                    typedef pre_order_iterator<not_const>          self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit pre_order_iterator(const data_type &cursor):base_class(cursor) {}
                    pre_order_iterator(const self_type &other):base_class(other.m_cursor) {}
                    
                    self_type& operator++(){
                        if(m_cursor==nullptr)
                            return *this;
                        if(m_cursor->left!=nullptr)
                            m_cursor=m_cursor->left;
                        else if(m_cursor->right!=nullptr)
                            m_cursor=m_cursor->right;
                        else{
                            data_type parent=m_cursor->parent;
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
            
            template <bool not_const> class in_order_iterator: public tree_iterator_base<not_const>{
                private:
                    typedef tree_iterator_base<not_const> base_class;
                public:
                    typedef in_order_iterator<not_const>           self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit in_order_iterator(const data_type &cursor):base_class(cursor) {
                        if(m_cursor==nullptr)
                            return;
                        while(m_cursor->left!=nullptr)
                            m_cursor=m_cursor->left;
                    }
                    in_order_iterator(const self_type &other):base_class(other.m_cursor) {}
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
                            data_type parent=m_cursor->parent;
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
            
            template <bool not_const> class post_order_iterator: public tree_iterator_base<not_const>{
                private:
                    typedef tree_iterator_base<not_const> base_class;
                public:
                    typedef post_order_iterator<not_const>         self_type;
                    typedef typename base_class::reference reference;
                    typedef typename base_class::data_type data_type;
                    
                    explicit post_order_iterator(const data_type &cursor):base_class(cursor) {
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
                    post_order_iterator(const self_type &other):base_class(other.m_cursor) {}
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
            template <bool not_const> class level_order_iterator:public iterator<std::forward_iterator_tag,typename condition<not_const,T,const T>::type> {
                private:
                    typedef iterator<std::forward_iterator_tag,typename condition<not_const,T,const T>::type> base_class;
                public:
                    typedef typename base_class::reference reference;
                    typedef typename base_class::pointer pointer;
                    
                    typedef level_order_iterator<not_const>                       self_type;
                    typedef typename condition<not_const,node*,const node*>::type data_type;
                    
                    explicit level_order_iterator(const data_type &cursor):m_cursor(nullptr) {
                        if(cursor!=nullptr){
                            m_cursor=std::make_shared<std::deque<data_type>>(std::deque<data_type>());
                            m_cursor->push_back(cursor);
                            // Make iteration doable starting at any node
                            data_type parent=cursor->parent;
                            if(parent!=nullptr && parent->left==cursor)
                                m_cursor->push_back(parent->right);
                        }
                    }
                    level_order_iterator(const self_type &other):m_cursor(other.m_cursor) {}
                    ~level_order_iterator() = default;
                    
                    reference operator*() const {return m_cursor->front()->data;}
                    pointer operator->() const { return &operator*(); }
                    data_type get() const {return m_cursor==nullptr?nullptr:m_cursor->front();}
                    self_type& operator++() {
                        if(m_cursor==nullptr||m_cursor->empty()){
                            m_cursor.reset();
                            return *this;
                        }
                        auto ptr=m_cursor->front();m_cursor->pop_front();
                        if(ptr->left != nullptr)
                            m_cursor->push_back(ptr->left);
                        if(ptr->right != nullptr)
                            m_cursor->push_back(ptr->right);
                        // Double check if the iteration comes to an end
                        if(m_cursor->empty())
                            m_cursor.reset();
                        return *this;
                    }
                    self_type operator++(int) {auto i=*this; operator++(); return i;}
                    bool operator==(const self_type &other) const {return m_cursor==other.m_cursor || (m_cursor!=nullptr && other.m_cursor!=nullptr && m_cursor->front()==other.m_cursor->front());}
                    bool operator!=(const self_type &other) const {return !operator==(other);}
                    self_type& operator=(const self_type &other) {m_cursor=other.m_cursor; return *this;}
                private:
                    std::shared_ptr< std::deque<data_type> > m_cursor;
            };
            
            typedef pre_order_iterator<true>    preorder_iterator;
            typedef in_order_iterator<true>     inorder_iterator;
            typedef post_order_iterator<true>   postorder_iterator;
            typedef level_order_iterator<true>  level_iterator;
            typedef pre_order_iterator<false>   const_preorder_iterator;
            typedef in_order_iterator<false>    const_inorder_iterator;
            typedef post_order_iterator<false>  const_postorder_iterator;
            typedef level_order_iterator<false> const_level_iterator;
