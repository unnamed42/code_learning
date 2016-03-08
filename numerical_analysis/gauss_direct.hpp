#ifndef __LINEAR_EQUATION__
#define __LINEAR_EQUATION__

#include "matrix.hpp"

class linear_equation: public matrix<double>{
    protected:
        using matrix<double>::elem;
        using matrix<double>::row;
        using matrix<double>::column;
    public:
        linear_equation();
        linear_equation(unsigned int _dim);
        void construct(unsigned int dim);
        matrix<double> solve();
};
linear_equation::linear_equation():matrix<double>::matrix(){}

linear_equation::linear_equation(unsigned int _dim):matrix<double>::matrix(_dim,_dim+1){}

linear_equation::construct(unsigned int dim){matrix<dobule>::construct(dim,dim+1);}

matrix<double> linear_equation::solve(){
    if(elem==nullptr)
        throw INVALID_OPERAND;
    matrix<double> result(row,1);
    for(int k=0;k<row-1;k++){
        if(elem[k][k]==0||elem[k+1][k]==0)
            continue;
        double coef=elem[k+1][k]/elem[k][k];
        for(int i=k+1;i<row;i++){
            for(int j=0;j<column;j++)
                elem[i][j]-=elem[k][j]*coef;
        }
    }
    std::cout<<*this;
    if(elem[row-1][row-1]==0)
        throw NO_SOLUTION;
    result[row-1][0]=elem[row-1][row]/elem[row-1][row-1];
    for(int i=row-2;i>=0;i--){
        double sum=0;
        for(int j=i+1;j<column-1;j++)
            sum+=elem[i][j]*result[j][0];
        result[i][0]=(elem[i][row]-sum)/elem[i][i];
    }
    return result;
}

#endif
