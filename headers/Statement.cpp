#ifndef STATEMENT_H
#define STATEMENT_H

#include "Expression.cpp"
#include "BBlock.cpp"


class Statement
{
  public:
    virtual void convert(BBlock** current, list<BBlock*> &functions) = 0;
};

#endif