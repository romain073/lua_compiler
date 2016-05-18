#ifndef STRING_H
#define STRING_H
#include "Expression.cpp"
#include <vector>
using namespace std;

class String: public Expression
{
public:
    string string_val;
      String(string t) 
    : string_val(t){
      type = Expression::types::STRING;
    }

    void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
      (*out)->instructions.push_back(ThreeAd(naming[this], "string", string_val, ""));
    }
};

#endif