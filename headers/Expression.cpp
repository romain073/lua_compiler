#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<string>
#include<map>
#include"BBlock.cpp"

#include<iostream>
#include<sstream>
using namespace std;

class Expression
{
public:
  enum types {UNDEFINED, BINOP, CONSTANT, VARIABLE, FUNCTIONCALL, STRING, UNOP, TABLE, TABLEACCESS, COMPARISON};
  types type;
  static int nameCounter;

  static string newName()
  {
    stringstream result;
    result << "_t" << Expression::nameCounter++;
    return result.str();
  }
  
  virtual void namePass(map<Expression*,string> &naming) = 0;
  
  virtual void emitPass(map<Expression*,string> &naming, BBlock** out) = 0;
  
  string convert(BBlock** out){
    map<Expression*,string> naming;
    this->namePass(naming);
    this->emitPass(naming, out);
    
    return naming[this];
  }
};


#endif