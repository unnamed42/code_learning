#ifndef __RUBBISH_PAIR__
#define __RUBBISH_PAIR__

#include <bits/move.h> // std::forward, std::move

namespace rubbish{
    
    template <class T1,class T2> struct pair{
        T1 first;
        T2 second;
        
        constexpr pair():first(),second() {}
        pair(const T1 &f,const T2 &s):first(f),second(s) {}
        pair(T1 &&f,T2 &&s):first(std::move(f)),second(std::move(s)) {}
        pair(const pair<T1,T2> &o):first(o.first),second(o.second) {}
        pair(pair<T1,T2> &&o):first(std::move(o.first)),second(std::move(o.second)) {}
        
        pair<T1,T2>& operator=(const pair<T1,T2> &o) {first=o.first;second=o.second;return *this;}
        pair<T1,T2>& operator=(pair<T1,T2> &&o) {first=std::move(o.first);second=std::move(o.second);return *this;}
    };
    
    template <class T1,class T2> bool operator==(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){return lhs.first==rhs.first&&lhs.second==rhs.second;}
    
    template <class T1,class T2> bool operator!=(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){return lhs.first!=rhs.first||lhs.second!=rhs.second;}
    
    template <class T1,class T2> bool operator<(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){
        if(lhs.first!=rhs.first)
            return lhs.first<rhs.first;
        return lhs.second<rhs.second;
    }
    
    template <class T1,class T2> bool operator>(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){return rhs>lhs;}
    
    template <class T1,class T2> bool operator<=(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){return !(lhs>rhs);}
    
    template <class T1,class T2> bool operator>=(const pair<T1,T2> &lhs,const pair<T1,T2> &rhs){return !(lhs<rhs);}
    
    template <class T1,class T2> pair<T1,T2> make_pair(T1 &&f,T2 &&s) {return pair<T1,T2>(std::forward(f),std::forward(s));}
    
} // namespace rubbish

#endif // __RUBBISH_PAIR__
