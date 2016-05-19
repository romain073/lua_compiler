#ifndef IF_H
#define IF_H
#include "Sequence.cpp"
#include "Statement.cpp"
#include<vector>

class If: public Statement
{
    public:
    Expression* condition;
    Sequence* block_true;
    Sequence* block_false;
    vector<pair<Expression*, Sequence*>> elsifs;
    If(Expression* condition, Sequence* instructions_true, Sequence* instructions_false, vector<pair<Expression*, Sequence*>> elsifs):
        condition(condition), block_true(instructions_true), block_false(instructions_false), elsifs(elsifs)
    {
    }
    
    ~If(){
        delete condition;
        delete block_true;
        delete block_false;
        
        for(auto i: elsifs){
            delete i.first;
            delete i.second;
        }
    }
    
    void convert(BBlock** current, list<BBlock*> &functions){
        condition->convert(current);
        
        BBlock* back = new BBlock();
        
        BBlock* true_block_begin = new BBlock();
        BBlock* true_block_end = true_block_begin;
        block_true->convert(&true_block_end, functions);
        (*current)->trueExit = true_block_begin;
        true_block_end->trueExit = back;
        

        
        for(auto i: elsifs){
            BBlock* elif_cond_begin = new BBlock();
            BBlock* elif_cond_end = elif_cond_begin;
            
            i.first->convert(&elif_cond_end);
            
            (*current)->falseExit = elif_cond_begin;
            BBlock* elif_block_begin = new BBlock();
            BBlock* elif_block_end = elif_block_begin;
            i.second->convert(&elif_block_end, functions);
            elif_block_end->trueExit = back;
            elif_cond_end->trueExit = elif_block_begin;
            
            *current = elif_cond_end;
        }
        
        
        if(block_false != NULL){
            BBlock* false_block_begin = new BBlock();
            BBlock* false_block_end = false_block_begin;
            block_false->convert(&false_block_end, functions);
            (*current)->falseExit = false_block_begin;
            false_block_end->trueExit = back;
        }else{
            (*current)->falseExit = back;
        }
        *current = back;
    }
};

#endif