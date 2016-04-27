#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<map>
#include"BBlock.cpp"

#include<iostream>
#include<sstream>
using namespace std;

class Expression      // Implicit union of binary operators, constants and variables.
{
public:

class Expression *left, *right;
int value;
string name;
static int nameCounter;
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
  
  static string newName()
  {
    stringstream result;
    result << "_t" << Expression::nameCounter++;
    return result.str();
  }
  
  virtual void namePass(map<Expression*,string> &naming){
    cout << "undefined namepass";
  }
  
  virtual void emitPass(map<Expression*,string> &naming, BBlock** out){
    
    cout << "undefined emit";
  }
  
  string convert(BBlock** out){
    map<Expression*,string> naming;
    this->namePass(naming);
    this->emitPass(naming, out);
    
    return naming[this];
  }
};


#endif