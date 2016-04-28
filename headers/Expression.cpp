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

  virtual void toString(){
      cout << "Expression" <<endl;
  }
  
  virtual bool isLeaf(){
    return left == NULL && right == NULL;
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