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
Expression *left, *right;
      Comparison(Expression *l, Expression *r, string t) 
    : op(t), left(l), right(r){
    }
    ~Comparison(){
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