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
      FunctionCall(Expression *name, vector<Expression*> args) 
    : Expression(name, NULL), args(args){
      type = Expression::types::FUNCTIONCALL;
    }
    
    void namePass(map<Expression*,string> &naming){
        naming[this] = Expression::newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
        string fnname = ((Variable*)this->left)->name;
        
        string names;
        for(auto arg : args){
            // Get the names and compute the arguments in the right order
            string name = arg->convert(out);
            names+=name+",";
        }
        names.pop_back(); // strip last comma
        
        string local_fn_name = fnname;
        if(fnname.compare("print") == 0 || fnname.compare("io.write") == 0){
            local_fn_name = "print";
        }
        
        // add the call
        (*out)->instructions.push_back(ThreeAd(naming[this], "call", local_fn_name, "("+names+")"));
        
        if(fnname.compare("print") == 0){
            // Add new line if print is called
            (*out)->instructions.push_back(ThreeAd("", "print_nl", "", ""));
        }
    }

};

#endif