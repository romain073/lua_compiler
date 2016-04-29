#ifndef UNOP_H
#define UNOP_H
#include "Expression.cpp"
using namespace std;

class UnOp: public Expression
{
public:
string op;
      UnOp(Expression *l, string t) 
    : Expression(l, NULL), op(t){}

  void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
        this->left->namePass(naming);
    }
    
 void emitPass(map<Expression*,string> &naming, BBlock** out){
    if (!this->left->isLeaf()) 
        this->left->emitPass(naming, out);

     
    ThreeAd a(naming[this], op, naming[this->left], "unary");
    (*out)->instructions.push_back(a);
  }
};

#endif