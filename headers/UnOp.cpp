#ifndef UNOP_H
#define UNOP_H
#include "Expression.cpp"
using namespace std;

class UnOp: public Expression
{
public:
    string op;
    Expression *operand;
      UnOp(Expression *l, string t) 
    : op(t), operand(l){
    }

  void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
        this->operand->namePass(naming);
    }
    
 void emitPass(map<Expression*,string> &naming, BBlock** out){
    this->operand->emitPass(naming, out);

    ThreeAd a(naming[this], op, naming[this->operand], "");
    (*out)->instructions.push_back(a);
  }
};

#endif