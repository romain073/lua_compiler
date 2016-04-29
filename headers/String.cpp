#ifndef STRING_H
#define STRING_H
#include "Expression.cpp"
#include <vector>
using namespace std;

class String: public Expression
{
public:
    static vector<pair<string,string>> strings;
    string string_val;
      String(string t) 
    : Expression(NULL, NULL), string_val(t){
    }

    void namePass(map<Expression*,string> &naming){
        naming[this] = newName(false); // disable regular number variable declaration
        strings.push_back(make_pair(naming[this], string_val));
    }
    
    void emitPass(map<Expression*,string> &naming, BBlock** out){}
};

#endif