#ifndef CONSTANT_H
#define CONSTANT_H
#include "Expression.cpp"
using namespace std;

class Constant: public Expression
{
public:
    string string_val;
      Constant(string t) 
    : string_val(t){
    }

    void namePass(map<Expression*,string> &naming){
        naming[this] = "$"+this->string_val;
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){}
};

#endif