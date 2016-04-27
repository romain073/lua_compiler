#ifndef IF_H
#define IF_H
#include "Sequence.cpp"
#include "Statement.cpp"

class If: public Statement
{
    public:
    Expression* condition;
    Sequence* seq_true;
    Sequence* seq_false;
    If(Expression* condition, Sequence* instructions_true, Sequence* instructions_false):
        condition(condition), seq_true(instructions_true), seq_false(instructions_false)
    {
    }
    
    void toString(){
        cout << "IF!" ;
        condition->toString();
        
        
        cout << endl;
    }
};

#endif