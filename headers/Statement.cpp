#ifndef STATEMENT_H
#define STATEMENT_H
#include<vector>
#include "Expression.cpp"
#include "BBlock.cpp"

class Statement
{
public:
vector<Expression*> expressions;
vector<Statement*> children;
  Statement() {}

  virtual void toString(){
      cout << "tostring"<<endl;
  }
  
  virtual void convert(BBlock** current){
    cout << "Convert not implemented"<<endl;
  }
};

#endif