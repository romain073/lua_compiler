#ifndef VARIABLE_H
#define VARIABLE_H
#include "Expression.cpp"
using namespace std;

class Variable: public Expression
{
public:
    string name;
      Variable(string t) 
    : name(t){
    }

    void namePass(map<Expression*,string> &naming){
        naming[this] = this->name;
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){  }
};

#endif