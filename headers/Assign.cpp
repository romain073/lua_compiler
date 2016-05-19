#ifndef ASSIGN_H
#define ASSIGN_H
#include "Statement.cpp"
#include <vector>

class Assign: public Statement
{
    public:
    vector<Expression*> variables;
    vector<Expression*> values;
    Assign(vector<Expression*> var, vector<Expression*> val): variables(var), values(val){ }
    
    ~Assign(){
        for(auto i: variables)
            delete i;
        for(auto i:values)
            delete i;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        if(this->variables.size()==1){
            // var = value
            string name = this->variables.front()->convert(current);
            string val = this->values.front()->convert(current);
            ThreeAd a(name, "assign", val, "");
            (*current)->instructions.push_back(a);
        }else{
            // var1, var2, ... = val1, val2, ...
            vector<string> tmp;
            for(size_t i = 0; i<this->variables.size(); i++){
                string tmp_name = Expression::newName();
                tmp.push_back(tmp_name);
                string val = this->values[i]->convert(current);
                ThreeAd a(tmp_name, "assign", val, "");
                (*current)->instructions.push_back(a);
            }
            
            for(size_t i = 0; i<this->variables.size(); i++){
                string tmp_name = tmp[i];
                string name = this->variables[i]->convert(current);
                ThreeAd a(name, "assign", tmp_name, "");
                (*current)->instructions.push_back(a);
            }
        }
    }
};

#endif