#ifndef COMPARISON_H
#define COMPARISON_H

#include "Expression.cpp"
#include "Variable.cpp"
#include "Constant.cpp"
using namespace std;

class Comparison: public Expression
{
public:

string op;
      Comparison(Expression *l, Expression *r, string t) 
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