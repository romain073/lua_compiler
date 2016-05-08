#ifndef WHILE_H
#define WHILE_H
#include "Sequence.cpp"
#include "Statement.cpp"
#include<vector>

class While: public Statement
{
    public:
    Expression* condition;
    Sequence* instructions;
    While(Expression* condition, Sequence* instructions):
        condition(condition), instructions(instructions)
    {
    }
    
    
    void convert(BBlock** current, list<BBlock*> &functions){
        condition->convert(current);
        
        BBlock* back = new BBlock();
        
        BBlock* loopblock = new BBlock();
        BBlock* loopcurr = loopblock;
        instructions->convert(&loopcurr, functions);
        
        condition->convert(&loopcurr);
        
        (*current)->trueExit = loopblock;
        (*current)->falseExit = back;
        loopcurr->trueExit = loopblock;
        loopcurr->falseExit = back;

        *current = back;
    }
};

#endif