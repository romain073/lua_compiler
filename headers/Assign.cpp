#ifndef ASSIGN_H
#define ASSIGN_H
#include "Statement.cpp"
#include <list>

class Assign: public Statement
{
    public:
    vector<Expression*> variables;
    vector<Expression*> values;
    Assign(vector<Expression*> var, vector<Expression*> val): variables(var), values(val){
        //t = target;
        // this->expressions.push_back( Variable(target) ); TODO
        //this->expressions.push_back(val);
    }
    
    void toString(){
        cout << "(Assign) ";
        for(size_t i = 0; i<this->variables.size(); i++){
            this->variables[i]->toString();
            cout  << ", ";
            this->values[i]->toString();
            cout << " - ";
        }
        cout << endl;
    }
};

#endif