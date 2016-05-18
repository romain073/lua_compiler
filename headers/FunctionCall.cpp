#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "Expression.cpp"
#include "Variable.cpp"
#include <vector>
using namespace std;

class FunctionCall: public Expression
{
public:
    vector<Expression*> args;
    Expression* name;
      FunctionCall(Expression *name, vector<Expression*> args) 
    : args(args), name(name){
    }
    
    void namePass(map<Expression*,string> &naming){
        naming[this] = Expression::newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
        string fnname = ((Variable*)this->name)->name;
        
        string names;
        for(auto arg : args){
            string name = arg->convert(out);
            names+=name+",";
        }
        names.pop_back(); // strip last comma
        
        // add the call
        (*out)->instructions.push_back(ThreeAd(naming[this], "call", fnname, "("+names+")"));
    }
};

#endif