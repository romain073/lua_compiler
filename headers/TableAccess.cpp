#ifndef TABLEACCESS_H
#define TABLEACCESS_H
#include "Expression.cpp"
using namespace std;

class TableAccess: public Expression
{
public:
    Expression* arr;
    Expression* idx;
      TableAccess(Expression* arr, Expression* idx) 
    : arr(arr), idx(idx){
    }
    
    ~TableAccess(){
      delete arr;
      delete idx;
    }

    void namePass(map<Expression*,string> &naming){
        arr->namePass(naming);
        idx->namePass(naming);
        naming[this] = newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
      idx->emitPass(naming, out);
      (*out)->instructions.push_back(ThreeAd(naming[this], "tableaccess", naming[arr], naming[idx]));
    }
};

#endif