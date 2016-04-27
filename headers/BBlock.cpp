#ifndef BBLOCK_H
#define BBLOCK_H



#include <list>
#include "ThreeAd.cpp"
#include<iostream>
#include<sstream>
#include<set>

using namespace std;
class BBlock
{
public:
list<ThreeAd> instructions;
BBlock *trueExit, *falseExit;
  
  BBlock()
    :  trueExit(NULL), falseExit(NULL)  {}

  void dump()
  {
    cout << "BBlock @ " << this << endl;
    for(auto i : instructions)
      i.dump();
    cout << "True: " << trueExit << endl;
    cout << "False: " << falseExit << endl;
  }
  
  void assembly(){
    cout << "_block" << this <<":"<< endl;
    for(auto i : instructions)
      i.assembly();
    if(trueExit != 0 && falseExit == 0)
      cout <<"jmp _block"<< trueExit << endl;
    else if(trueExit != 0){
    cout <<"_block"<< trueExit << endl;
    cout << "jmp _block" << falseExit << endl;
    }
  }
  
  void dumpCFG()
{
set<BBlock *> done, todo;
  todo.insert(this);
  while(todo.size()>0)
  {
    // Pop an arbitrary element from todo set
    auto first = todo.begin();
    BBlock *next = *first;
    todo.erase(first);

    next->dump();
    done.insert(next);
    if(next->trueExit!=NULL && done.find(next->trueExit)==done.end())
      todo.insert(next->trueExit);
    if(next->falseExit!=NULL && done.find(next->falseExit)==done.end())
      todo.insert(next->falseExit);
  }
}
  
  
};
#endif