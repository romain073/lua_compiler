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
    
    ~FunctionCall(){
        for(auto i: args)
            delete i;
        delete name;
    }
    
    void namePass(map<Expression*,string> &naming){
        naming[this] = Expression::newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
        string names;
        for(auto arg : args){
            names+=arg->convert(out)+",";
        }
        names.pop_back(); // strip last comma
        
        // add the call
        (*out)->instructions.push_back(ThreeAd(naming[this], "call", ((Variable*)this->name)->name, "("+names+")"));
    }
};

#endif