#ifndef COMPARISON_H
#define COMPARISON_H
#include "Expression.cpp"
using namespace std;

class Comparison: public Expression
{
public:
char op;
      Comparison(Expression *l, Expression *r, char t) 
    : Expression(l, r), op(t){}

  void toString(){
      cout << "Comparison " << op;
      left->toString();
      cout << " ";
      right->toString();
    
  }
  void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
        this->left->namePass(naming);
        this->right->namePass(naming);
    }
    
 void emitPass(map<Expression*,string> &naming, BBlock** out){
    ThreeAd a(naming[this], op, naming[this->left], naming[this->right]);
    (*out)->instructions.push_back(a);
  }
};

#endif