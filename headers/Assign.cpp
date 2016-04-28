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
    
    void convert(BBlock** current){
        if(this->variables.size()==1){
            // var = value
            string name = this->variables.front()->convert(current);
            string val = this->values.front()->convert(current);
            ThreeAd a(name, 'c', val, val);
            (*current)->instructions.push_back(a);
        }else{
            // var1, var2, ... = val1, val2, ...
            vector<string> tmp;
            for(size_t i = 0; i<this->variables.size(); i++){
                string tmp_name = Expression::newName();
                tmp.push_back(tmp_name);
                string val = this->values[i]->convert(current);
                ThreeAd a(tmp_name, 'c', val, val);
                (*current)->instructions.push_back(a);
            }
            
            for(size_t i = 0; i<this->variables.size(); i++){
                string tmp_name = tmp[i];
                string name = this->variables[i]->convert(current);
                ThreeAd a(name, 'c', tmp_name, tmp_name);
                (*current)->instructions.push_back(a);
            }
        }
    }

};

#endif