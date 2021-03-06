#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Statement.cpp"

class Sequence: public Statement
{
    public:
    vector<Statement*> children;
    Sequence(){
    }
    
    ~Sequence(){
        for(auto s : this->children){
            delete s;
        }
    }
    
    Sequence* add(Statement* s){
        this->children.push_back(s);
        return this;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        for(auto s : this->children){
            s->convert(current, functions);
        }
    }
};

#endif