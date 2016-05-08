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
        /*
        if((fnname.compare("print") == 0 || fnname.compare("io.write") == 0)
                && args.front()->memberOf(Expression::types::STRING)){
            // assume that if the first arg is a string, the others are as well.
            for(auto arg : args){
                string name = arg->convert(out);
                (*out)->instructions.push_back(ThreeAd("", "print", name, ""));
            }
            if(fnname.compare("print") == 0){
                // Add new line if print is called
                (*out)->instructions.push_back(ThreeAd("", "print_nl", "", ""));
            }
            return;
        }
        */
        
        vector<string> names;
        for(auto arg : args){
            // Get the names and compute the arguments in the right order
            string name = arg->convert(out);
            names.push_back(name);
        }
        
        for (size_t i = names.size(); i-- > 0; ) {
            // Push the arguments in reverse order
            (*out)->instructions.push_back(ThreeAd("", "argv", names[i], ""));
        }
        
        string local_fn_name = fnname;
        if(fnname.compare("print") == 0 || fnname.compare("io.write") == 0){
            local_fn_name = "print";
        }
        
        // add the call
        (*out)->instructions.push_back(ThreeAd(naming[this], "call", local_fn_name, ""));
        
        if(fnname.compare("print") == 0){
            // Add new line if print is called
            (*out)->instructions.push_back(ThreeAd("", "print_nl", "", ""));
        }
    }

};

#endif