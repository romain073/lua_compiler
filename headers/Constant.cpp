#ifndef CONSTANT_H
#define CONSTANT_H
#include "Expression.cpp"
using namespace std;

class Constant: public Expression
{
public:
string val;
      Constant(string t) 
    : Expression(NULL, NULL), val(t){}

  void toString(){
      cout << "(Constant)'" <<val <<"'";
  }
};

#endif