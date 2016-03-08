#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>

typedef enum{INVALID_OPERAND=0,NO_SOLUTION=1} err_t;

template <class T> class matrix{
    protected:
        int row,column;
        T **elem;
        void init(int row,int column);
    public:
        explicit matrix();
        explicit matrix(int dim);
        explicit matrix(int r,int c);
        matrix(const matrix<T> &m);
        ~matrix();
        void input();
        void construct(int row,int cloumn);
        void transpose();
        float determinate() const;
        bool operator==(const matrix<T> &m) const;
        T* operator[](unsigned int n) const;
        matrix<T> operator+(const matrix<T> &m) const;
        matrix<T> operator-(const matrix<T> &m) const;
        matrix<T> operator*(const T &coef) const;
        matrix<T> operator*(const matrix<T> &m) const;
        matrix<T>& operator=(const matrix<T> &m);
        matrix<T>& operator+=(const matrix<T> &m);
        matrix<T>& operator-=(const matrix<T> &m);
        matrix<T>& operator*=(const T &coef);
        
        static float determinate(const matrix<T> &m);
        template <class S> friend std::ostream &operator<<(std::ostream &stream,const matrix<S> &m);
};
template <class T> void matrix<T>::init(int row,int column){
    this->row=row;this->column=column;
    elem=new T*[row]();
    for(int i=0;i<row;i++)
        elem[i]=new T[column]();
}

template <class T> matrix<T>::matrix():row(0),column(0),elem(nullptr){}

template <class T> matrix<T>::matrix(int dim){init(dim,dim);}

template <class T> matrix<T>::matrix(int r, int c){init(r,c);}

template <class T> matrix<T>::matrix(const matrix<T> &m){
    row=m.row;column=m.column;
    elem=new T*[row]();
    for(int i=0;i<row;i++){
        elem[i]=new T[column]();
        for(int j=0;j<column;j++)
            elem[i][j]=m.elem[i][j];
    }
}

template <class T> matrix<T>::~matrix(){
    for(int i=0;i<row;i++)
        delete[] elem[i];
    delete[] elem;
}

template <class T> void matrix<T>::input(){
    std::cout<<"Input "<<row<<"x"<<column<<" integers:"<<std::endl;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            std::cin>>elem[i][j];
    }
}

template <class T> void matrix<T>::construct(int row,int column){
    init(row,column);
    input();
}

template <class T> float matrix<T>::determinate() const {return determinate(*this);}

template <class T> void matrix<T>::transpose(){
    matrix<T> temp(column,row);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            temp.elem[j][i]=elem[i][j];
    }
    *this=temp;
}

template <class T> bool matrix<T>::operator==(const matrix<T> &m) const {
    if(row!=m.row||column!=m.column)
        return false;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            if(elem[i][j]!=m.elem[i][j])
                return false;
        }
    }
    return true;
}

template <class T> T* matrix<T>::operator[](unsigned int n) const {return elem[n];}

template <class T> matrix<T> matrix<T>::operator+(const matrix<T> &m) const {
    if(row!=m.row||column!=m.column)
        throw INVALID_OPERAND;
    matrix temp(*this);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            temp.elem[i][j]+=m.elem[i][j];
    }
    return temp;
}

template <class T> matrix<T> matrix<T>::operator-(const matrix<T> &m) const {
    if(row!=m.row||column!=m.column)
        throw INVALID_OPERAND;
    matrix temp(*this);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            temp.elem[i][j]-=m.elem[i][j];
    }
    return temp;
}

template <class T> matrix<T> matrix<T>::operator*(const T &coef) const {
    if(elem==nullptr)
        throw INVALID_OPERAND;
    matrix<T> temp(*this);
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            temp.elem[i][j]*=coef;
    }
    return temp;
}

template <class T> matrix<T> matrix<T>::operator*(const matrix<T> &m) const {
    if(elem==nullptr||m.elem==nullptr||column!=m.row)
        throw INVALID_OPERAND;
    matrix<T> result(row,m.column);
    for(int i=0;i<row;i++){
        for(int j=0;j<m.column;j++){
            for(int k=0;k<column;k++)
                result[i][j]+=elem[i][k]*m.elem[k][j];
        }
    }
    return result;
}

template <class T> matrix<T>& matrix<T>::operator=(const matrix<T> &m){
    if(elem!=nullptr){
        for(int i=0;i<row;i++)
            delete[] elem[i];
        delete[] elem;
    }
    row=m.row;column=m.column;
    elem=new T*[row];
    for(int i=0;i<row;i++){
        elem[i]=new T[column];
        for(int j=0;j<column;j++)
            elem[i][j]=m.elem[i][j];
    }
    return *this;
}

template <class T> matrix<T>& matrix<T>::operator+=(const matrix<T> &m){
    if(row!=m.row||column!=m.column)
        throw INVALID_OPERAND;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            elem[i][j]+=m.elem[i][j];
        }
    }
    return *this;
}

template <class T> matrix<T>& matrix<T>::operator-=(const matrix<T> &m){
    if(row!=m.row||column!=m.column)
        throw INVALID_OPERAND;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++){
            elem[i][j]-=m.elem[i][j];
        }
    }
    return *this;
}

template <class T> matrix<T>& matrix<T>::operator*=(const T &coef){
    if(elem==nullptr)
        throw INVALID_OPERAND;
    for(int i=0;i<row;i++){
        for(int j=0;j<column;j++)
            elem[i][j]*=coef;
    }
    return *this;
}

template <class T> float matrix<T>::determinate(const matrix<T> &m) {
    if(m.row!=m.column)
        throw INVALID_OPERAND;
    if(m.row==1)
        return elem[0][0];
    if(m.row==2)
        return (elem[0][0]*elem[1][1]-elem[0][1]*elem[1][0]);
    float det=0;int p,h=0,k=0;
    matrix<float> temp(m.row-1);
    for(p=0;p<m.row;p++){
        for(int i=1,j=0;i<m.row;i++){
            for(;j<m.row;j++){
                if(j==p)
                    continue;
            }
            temp.elem[h][k++]=m.elem[i][j];
            if(k==m.row-1)
                h++,k=0;
        }
    }
    det+=m.elem[0][p]*(p%2==0?1:-1)*determinate(temp);
    return det;
}

template <class T> std::ostream& operator<<(std::ostream &stream,const matrix<T> &m){
    for(int i=0;i<m.row;i++){
        for(int j=0;j<m.column;j++)
            stream<<m.elem[i][j]<<" ";
        stream<<std::endl;
    }
    return stream;
}

#endif
