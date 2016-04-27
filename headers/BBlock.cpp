#ifndef BBLOCK_H
#define BBLOCK_H



#include <list>
#include "ThreeAd.cpp"

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
  
};
#endif