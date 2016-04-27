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
    
    
    void convert(BBlock** current){
        // TODO convert compar to current block
        
        (*current)->instructions.push_back(ThreeAd("todo", 'c', "comp", "comp"));
        
        BBlock* back = new BBlock();
        
        BBlock* loopblock = new BBlock();
        instructions->convert(&loopblock);
        
        // TODO convert compar to loop block
        loopblock->instructions.push_back(ThreeAd("todo", 'c', "comp", "comp"));
        
        (*current)->trueExit = loopblock;
        (*current)->falseExit = back;
        loopblock->trueExit = loopblock;
        loopblock->falseExit = back;

        *current = back;
    }
};

#endif