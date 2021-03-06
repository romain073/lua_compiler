#ifndef BINOP_H
#define BINOP_H
#include "Expression.cpp"
using namespace std;

class BinOp: public Expression
{
public:
    string op;
    
    Expression *left, *right;
      BinOp(Expression *l, Expression *r, string t) 
    : op(t), left(l), right(r){
    }
    ~BinOp(){
      delete left;
      delete right;
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