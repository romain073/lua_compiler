#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "Expression.cpp"
using namespace std;

class FunctionCall: public Expression
{
public:

      FunctionCall(Expression *name) 
    : Expression(name, NULL){}
    
    void namePass(map<Expression*,string> &naming){
        this->left->namePass(naming);
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
        ThreeAd a("", 'x', naming[this->left], "");
        (*out)->instructions.push_back(a);
    }

};

#endif