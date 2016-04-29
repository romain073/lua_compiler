#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "Expression.cpp"
#include <vector>
using namespace std;

class FunctionCall: public Expression
{
public:
    vector<Expression*> args;
      FunctionCall(Expression *name, vector<Expression*> args) 
    : Expression(name, NULL), args(args){}
    
    void namePass(map<Expression*,string> &naming){
        this->left->namePass(naming);
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
        vector<string> names;
        for(auto arg : args){
            // Get the names and compute the arguments in the right order
            string name = arg->convert(out);
            names.push_back(name);
        }
        
        for (size_t i = names.size(); i-- > 0; ) {
            // Push the arguments in reverse order
            (*out)->instructions.push_back(ThreeAd("", "push", names[i], ""));
        }
        
        // Push argc
        (*out)->instructions.push_back(ThreeAd("", "push", "$"+to_string(names.size()), ""));
        
        // add the call
        (*out)->instructions.push_back(ThreeAd("", "x", naming[this->left], ""));
    }

};

#endif