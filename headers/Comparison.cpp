#ifndef COMPARISON_H
#define COMPARISON_H
#include "Expression.cpp"
#include "Variable.cpp"
#include "Constant.cpp"
using namespace std;

class Comparison: public Expression
{
public:

enum Operation {EQ,NE,GE,GT,LE,LT};
const string opStrings[6] = { "EQ","NE","GE","GT","LE","LT" };

Operation op;
      Comparison(Expression *l, Expression *r, Operation t) 
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
    if (!this->left->isLeaf()) 
        this->left->emitPass(naming, out);
    if (!this->right->isLeaf()) 
        this->right->emitPass(naming, out);

     
    ThreeAd a(naming[this], 48+op, naming[this->left], naming[this->right] +" //"+ opStrings[op]);
    (*out)->instructions.push_back(a);
  }
};

#endif