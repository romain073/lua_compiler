#ifndef BINOP_H
#define BINOP_H
#include "Expression.cpp"
using namespace std;

class BinOp: public Expression
{
public:
char op;
      BinOp(Expression *l, Expression *r, char t) 
    : Expression(l, r), op(t){}

  void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
        this->left->namePass(naming);
        this->right->namePass(naming);
    }
    
 void emitPass(map<Expression*,string> &naming, BBlock** out){
    if (!this->left->isLeaf()) 
        this->left->emitPass(naming, out);
    if (!this->right->isLeaf()) 
        this->right->emitPass(naming, out);

     
    ThreeAd a(naming[this], op, naming[this->left], naming[this->right]);
    (*out)->instructions.push_back(a);
  }
};

#endif