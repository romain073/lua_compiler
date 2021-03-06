#ifndef FUNCTIONDEF_H
#define FUNCTIONDEF_H
#include "Sequence.cpp"
#include "Statement.cpp"
#include<vector>

class FunctionDef: public Statement
{
    public:
    string name;
    list<string> params;
    Sequence* body;
    FunctionDef(string name, list<string> params, Sequence* body): name(name), params(params), body(body)
    { }
    
    ~FunctionDef(){
        delete body;
    }
    
    void setName(string n){
        name = n;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        BBlock* fn = new BBlock(name);
        BBlock* curr_fn = fn;
        for(string i : params)
            fn->instructions.push_back(ThreeAd("", "param", i, ""));
        
        body->convert(&curr_fn, functions);
        
        functions.push_back(fn);
    }
};

#endif