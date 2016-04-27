#ifndef VARIABLE_H
#define VARIABLE_H
#include "Expression.cpp"
using namespace std;

class Variable: public Expression
{
public:
string val;
      Variable(string t) 
    : Expression(NULL, NULL), val(t){}

  void toString(){
      cout << "(Variable)'" <<val <<"'";
  }
};

#endif