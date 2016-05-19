#ifndef RETURN_H
#define RETURN_H
#include "Statement.cpp"

class Return: public Statement
{
    public:
    vector<Expression*> e;
    Return(vector<Expression*> e) :e(e) {}
    
    ~Return(){
        for(auto i: e)
            delete i;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        string name;
        for(auto i:e)
            name = i->convert(current);
        (*current)->instructions.push_back(ThreeAd("", "return", name, ""));
    }
};

#endif