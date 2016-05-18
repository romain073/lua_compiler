#ifndef BINOP_H
#define BINOP_H
#include "Expression.cpp"
using namespace std;

class BinOp: public Expression
{
public:
        string op;
      BinOp(Expression *l, Expression *r, string t) 
    : Expression(l, r), op(t){
        
      type = Expression::types::BINOP;
    }

  void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
        this->left->namePass(naming);
        this->right->namePass(naming);
    }
    
 void emitPass(map<Expression*,string> &naming, BBlock** out){
    this->left->emitPass(naming, out);
    this->right->emitPass(naming, out);

    ThreeAd a(naming[this], op, naming[this->left], naming[this->right]);
    (*out)->instructions.push_back(a);
  }
};

#endif