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
    
    void setName(string n){
        name = n;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        cout << "Defined function " << name << endl;
                for(auto i : params){
            cout << "p "<<i<<endl;
        }
    }
};

#endif