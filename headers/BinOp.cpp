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

  void toString(){
      cout << "BinOp " << op;
      left->toString();
      cout << " ";
      right->toString();
    
  }
};

#endif