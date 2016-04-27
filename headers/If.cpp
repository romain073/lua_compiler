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
    
    void toString(){
        cout << "IF!" ;
        condition->toString();
        
        
        cout << endl;
    }
    
    void convert(BBlock** current){
        // TODO convert compar
        
        
        BBlock* back = new BBlock();
        
        BBlock* trueBlock = new BBlock();
        seq_true->convert(&trueBlock);
        (*current)->trueExit = trueBlock;
        trueBlock->trueExit = back;
        /*
        for(auto i: elsifs){
            // i.first/ i.second
            BBlock* b = new BBlock();
            seq_true->convert(&trueBlock);
            (*current)->trueExit = trueBlock;
            trueBlock->trueExit = back;
        }
        */
        
        if(seq_false != NULL){
            BBlock* falseBlock = new BBlock();
            seq_false->convert(&falseBlock);
            (*current)->falseExit = falseBlock;
            falseBlock->trueExit = back;
        }
        *current = back;
    }
};

#endif