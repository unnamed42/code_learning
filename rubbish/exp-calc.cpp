#include <iostream>
#include <cstdlib>
#include <cstring>

#define isnumber(a) ((a)>='0'&&(a)<='9')

template <class T> class stack{
    private:
        T *base;
        T *top;
        int size;
    public:
        stack();
        stack(int _size);
        ~stack();
        void resize(int _size);
        void push(T data);
        bool empty();
        T pop();
        T get_top();
};
template <class T> stack<T>::stack():base(NULL),top(NULL),size(0){}
template <class T> stack<T>::stack(int _size):base(new T[_size]()),top(NULL),size(_size){top=base;}
template <class T> stack<T>::~stack(){delete[] base;}
template <class T> bool stack<T>::empty(){return top==base;}
template <class T> void stack<T>::resize(int _size){
    T *temp=(T*)realloc(base,_size*sizeof(T));
    if(temp==NULL){
        std::cerr<<"Not enough memory."<<std::endl;
        return;
    }
    base=temp;size=_size;
    if(top==NULL)
        top=base;
}
template <class T> void stack<T>::push(T data){
    if(top-base>=size)
        resize(size+10);
    *(top++)=data;
}
template <class T> T stack<T>::pop(){
    if(top!=base)
        return *(--top);
    std::cerr<<"Stack empty!"<<std::endl;
    return T(-1);
}
template <class T> T stack<T>::get_top(){
    if(top!=base)
        return *(top-1);
    std::cerr<<"Stack empty!"<<std::endl;
    return T(-1);
}

int priority(char a,char b){
    switch(a){
        case '+':
        case '-':return (b=='+'||b=='-'||b==')')?1:0;
        case '*':
        case '/':return (b=='(')?0:1;
        case '(':return 2;
    }
}

char *postfix(const char *exp){
    char *result=new char[strlen(exp)+1]();stack<char> s;
    char *temp=result;
    while(*exp!='\0'){
        if(isnumber(*exp))
            *(temp++)=*(exp++);
        else if(*exp==')'){
            while(s.get_top()!='(')
                *(temp++)=s.pop();
            s.pop();exp++;
        }
        else{
            if(!s.empty()&&priority(s.get_top(),*exp)==1){
                do{
                    *(temp++)=s.pop();
                }while(!s.empty()&&priority(s.get_top(),*exp)==1);
            }
            s.push(*exp);exp++;
        }
    }
    while(!s.empty())
        *(temp++)=s.pop();
    *temp='\0';
    return result;
}

float calculate(const char *post_exp){
    stack<float> s(10);float num_l=0,num_r=0;
    while(*post_exp!='\0'){
        if(isnumber(*post_exp))
            s.push(float(*post_exp-'0'));
        else{
            num_r=s.pop();num_l=s.pop();
            switch(*post_exp){
                case '+':s.push(num_l+num_r);break;
                case '-':s.push(num_l-num_r);break;
                case '*':s.push(num_l*num_r);break;
                case '/':
                    if(num_r==0)
                        std::cerr<<"Error!"<<std::endl;
                    s.push(num_l/num_r);break;
                default:std::cerr<<"Error!"<<std::endl;
            }
        }
        post_exp++;
    }
    return s.get_top();
}

int main(){
    char *exp=postfix("4+3*9/2-(1*2+1)");
    std::cout<<exp<<std::endl;
    std::cout<<calculate(exp)<<std::endl;
    return 0;
} 
