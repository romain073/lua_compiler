#ifndef STATEMENT_H
#define STATEMENT_H
#include<vector>
#include "Expression.cpp"

class Statement
{
public:
vector<Expression*> expressions;
vector<Statement*> children;
  Statement() {}

  void dump(int indent=0)
  {
    for(int i=0; i<indent; i++)
      cout << "  ";
    
    this->toString();
    
    for( auto e: expressions )
      e->dump(indent+1);
    for( auto c: children )
      c->dump(indent+1);
    

  }
  
  virtual void toString(){
      cout << "tostring"<<endl;
  }
};

#endif