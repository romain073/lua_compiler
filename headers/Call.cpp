#ifndef CALL_H
#define CALL_H
#include "Statement.cpp"
#include <vector>

class Call: public Statement
{
    public:
    Expression* exp;
    Call(Expression* exp): exp(exp){ }
    
    void convert(BBlock** current){
        this->exp->convert(current);
    }
};

#endif