#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>

#include<iostream>
using namespace std;

class Expression      // Implicit union of binary operators, constants and variables.
{
public:
class Expression *left, *right;
int value;
string name;

  Expression(Expression *l, Expression *r) 
    : left(l), right(r){}

  void dump(int depth=0)
  {
    for(int i=0; i<depth; i++)  
      cout << "  ";
    this->toString();
    if(left!=NULL)
      left->dump(depth+1);
    if(right!=NULL)
      right->dump(depth+1);
  }
  
  virtual void toString(){
      cout << "Expression" <<endl;
  }
};

#endif