#include <iostream>
#include <iomanip>
#include <ios>

using namespace std;

class polynomial;

class polynomial_node{
    friend class polynomial;
    private:
        float coef;
        int expn;
        polynomial_node *next;
    public:
        polynomial_node():coef(0.0),expn(0),next(NULL){}
        polynomial_node(float f,int i):coef(f),expn(i),next(NULL){}
        polynomial_node(polynomial_node &p){
            coef=p.coef;expn=p.expn;next=NULL;
        }
        friend ostream& operator<<(ostream &stream,polynomial_node &p){
            stream<<setiosflags(ios::fixed)<<setiosflags(ios::showpos)<<setprecision(2)<<p.coef;
            stream.unsetf(ios::fixed);
            stream.unsetf(ios::showpos);
            stream<<"x^"<<p.expn;
            return stream;
        }
        friend ostream& operator<<(ostream &stream,polynomial &p);
};

class polynomial{
    private:
        polynomial_node *head;
    public:
        polynomial():head(NULL){}
        polynomial(polynomial &p){
            head=new polynomial_node;
            polynomial_node *temp=p.head,**node=&head->next;
            while(temp!=NULL){
                *node=new polynomial_node(*temp);
                node=&(*node)->next;
                temp=temp->next;
            }
        }
        ~polynomial(){
            polynomial_node *node=head,*save;
            while(node!=NULL){
                save=node->next;
                delete node;
                node=save;
            }
            head=NULL;
        }
        void append(float co,int ex){
            polynomial_node *p=head,*q;
            while(p->next!=NULL)
                p=p->next;
            q=new polynomial_node(co,ex);
            p->next=q;
        }
        void input(){
            polynomial_node **temp;float co=0;int ex=0;
            head=new polynomial_node;
            if(head==NULL){
                cerr<<"Error!"<<endl;
                return;
            }
            temp=&head->next;
            cout<<"请输入数据(格式：系数 指数，输入-1 3停止)，按指数从小到大顺序输入\n";
            while(1){
                cin>>co>>ex;
                if(co==0&&ex==0)
                    break;
                *temp=new polynomial_node;
                if(*temp==NULL){
                    cerr<<"Error!"<<endl;
                    return;
                }
                (*temp)->coef=co;(*temp)->expn=ex;
                temp=&(*temp)->next;
            }
            *temp=NULL;
        }
        polynomial& operator+(polynomial &p){
            polynomial *_new=new polynomial(*this);
            polynomial_node *p1=_new->head,*p2=p.head;
            p2=p2->next;
            while(p1->next!=NULL&&p2!=NULL){
                if(p2->expn > p1->next->expn)
                    p1=p1->next;
                else if(p2->expn < p1->next->expn){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
                else{
                    p1->next->coef+=p2->coef;
                    p1=p1->next;
                    p2=p2->next;
                }
            }
            if(p1->next==NULL){//this is why I use p1->next to replace p1 above.
                while(p2!=NULL){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
            }
            return *_new;
        }
        polynomial& operator-(polynomial &p){
            polynomial *_new=new polynomial(*this);
            polynomial_node *p1=_new->head,*p2=p.head;
            p2=p2->next;
            while(p1->next!=NULL&&p2!=NULL){
                if(p2->expn > p1->next->expn)
                    p1=p1->next;
                else if(p2->expn < p1->next->expn){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
                else{
                    p1->next->coef-=p2->coef;
                    p1=p1->next;
                    p2=p2->next;
                }
            }
            if(p1->next==NULL){//this is why I use p1->next to replace p1 above.
                while(p2!=NULL){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    temp->coef*=-1;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
            }
            return *_new;
        }
        polynomial& operator+=(polynomial &p){
            polynomial_node *p1=head,*p2=p.head;
            p2=p2->next;
            while(p1->next!=NULL&&p2!=NULL){
                if(p2->expn > p1->next->expn)
                    p1=p1->next;
                else if(p2->expn < p1->next->expn){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
                else{
                    p1->next->coef+=p2->coef;
                    p1=p1->next;
                    p2=p2->next;
                }
            }
            if(p1->next==NULL){//this is why I use p1->next to replace p1 above.
                while(p2!=NULL){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
            }
            return *this;
        }
        polynomial& operator-=(polynomial &p){
            polynomial_node *p1=head,*p2=p.head;
            p2=p2->next;
            while(p1->next!=NULL&&p2!=NULL){
                if(p2->expn > p1->next->expn)
                    p1=p1->next;
                else if(p2->expn < p1->next->expn){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
                else{
                    p1->next->coef-=p2->coef;
                    p1=p1->next;//if p1=p2 and already added. p2=next will be operated below.
                    p2=p2->next;
                }
            }
            if(p1->next==NULL){//this is why I use p1->next to replace p1 above.
                while(p2!=NULL){
                    polynomial_node *temp=new polynomial_node(*p2);
                    temp->coef*=-1;
                    temp->next=p1->next;
                    p1->next=temp;
                    p1=temp;
                    p2=p2->next;
                }
            }
            return *this;
        }
        friend ostream& operator<<(ostream &stream,polynomial &p){
            polynomial_node *_head=p.head;
            _head=_head->next;
            while(_head!=NULL){
                stream<<(*_head);
                _head=_head->next;
            }
            stream<<endl;
            return stream;
        }
};

int main(){
    polynomial p1,p2;
    p1.input();
    p2.input();
    cout<<p1;
    cout<<p2;
    cout<<(p1+p2);
    cout<<"Terminated successfully.";
    return 0;
} 
