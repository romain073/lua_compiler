#ifndef TABLE_H
#define TABLE_H
#include "Expression.cpp"
#include "Constant.cpp"
#include <vector>
using namespace std;

class Table: public Expression
{
public:
    string table_val;
      Table(vector<Expression*> t) {
      type = Expression::types::TABLE;
      for(auto i: t){
        table_val+=((Constant*)i)->string_val + ",";
      }
      table_val.pop_back();// remove trailing comma
    }

    void namePass(map<Expression*,string> &naming){
        naming[this] = newName();
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){
      (*out)->instructions.push_back(ThreeAd(naming[this], "table", table_val, ""));
    }
};

#endif