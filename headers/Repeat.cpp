#ifndef REPEAT_H
#define REPEAT_H
#include "Sequence.cpp"
#include "Statement.cpp"
#include<vector>

class Repeat: public Statement
{
    public:
    Expression* condition;
    Sequence* instructions;
    Repeat(Expression* condition, Sequence* instructions):
        condition(condition), instructions(instructions)
    {
    }
    
    
    void convert(BBlock** current){
        
        BBlock* back = new BBlock();
        
        BBlock* loopblock = new BBlock();
        BBlock* loopcurr = loopblock;
        instructions->convert(&loopcurr);
        
        condition->convert(&loopcurr);
        
        (*current)->trueExit = loopblock;
        loopcurr->trueExit = back;
        loopcurr->falseExit = loopblock;

        *current = back;
    }
};

#endif