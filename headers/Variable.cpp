#ifndef VARIABLE_H
#define VARIABLE_H
#include "Expression.cpp"
using namespace std;

class Variable: public Expression
{
public:
string val;
      Variable(string t) 
    : Expression(NULL, NULL), val(t){}

    void namePass(map<Expression*,string> &naming){
        naming[this] = this->val;
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){  }
};

#endif