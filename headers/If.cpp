#ifndef IF_H
#define IF_H
#include "Sequence.cpp"
#include "Statement.cpp"
#include<vector>

class If: public Statement
{
    public:
    Expression* condition;
    Sequence* seq_true;
    Sequence* seq_false;
    vector<pair<Expression*, Sequence*>> elsifs;
    If(Expression* condition, Sequence* instructions_true, Sequence* instructions_false, vector<pair<Expression*, Sequence*>> elsifs):
        condition(condition), seq_true(instructions_true), seq_false(instructions_false), elsifs(elsifs)
    {
    }
    
    void convert(BBlock** current){
        condition->convert(current);
        
        BBlock* back = new BBlock();
        
        BBlock* trueBlock = new BBlock();
        BBlock* trueCur = trueBlock;
        seq_true->convert(&trueCur);
        (*current)->trueExit = trueBlock;
        trueCur->trueExit = back;
        /*
        for(auto i: elsifs){
            // i.first/ i.second
        }
        */
        
        if(seq_false != NULL){
            BBlock* falseBlock = new BBlock();
            BBlock* falseCur = falseBlock;
            seq_false->convert(&falseCur);
            (*current)->falseExit = falseBlock;
            falseCur->trueExit = back;
        }else{
            (*current)->falseExit = back;
        }
        *current = back;
    }
};

#endif