#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Statement.cpp"

class Sequence: public Statement
{
    public:
    Sequence(){
    }
    
    Sequence* add(Statement* s){
        this->children.push_back(s);
        return this;
    }
    
    void toString(){
        cout << "SEQUENCE!" << endl;
    }
};

#endif