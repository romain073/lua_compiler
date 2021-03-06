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
    
    ~Repeat(){
        delete condition;
        delete instructions;
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        
        BBlock* back = new BBlock();
        
        BBlock* loopblock = new BBlock();
        BBlock* loopcurr = loopblock;
        instructions->convert(&loopcurr, functions);
        
        condition->convert(&loopcurr);
        
        (*current)->trueExit = loopblock;
        loopcurr->trueExit = back;
        loopcurr->falseExit = loopblock;

        *current = back;
    }
};

#endif