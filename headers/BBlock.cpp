#ifndef BBLOCK_H
#define BBLOCK_H



#include <list>
#include "ThreeAd.cpp"
#include<iostream>
#include<sstream>
#include<fstream>
#include<set>

using namespace std;
class BBlock
{
public:
list<ThreeAd> instructions;
BBlock *trueExit, *falseExit;
string label;
static int blockCounter;
  
  BBlock()
    :  trueExit(NULL), falseExit(NULL)  {
      label = newName();
    }


  void blockToGraph(ofstream &f){
    f << label << "[label=\"{<h>"<< label <<"|";
    for(auto i : instructions){
      i.graph(f);
      f<<"|";
    }
    if(trueExit != NULL)
      f << "<t>True: " << trueExit->label;
    else
      f << "True: NULL";
    f<<"|";
    if(falseExit != NULL)
      f << "<f>False: " << falseExit->label;
    else
      f << "False: NULL";
    f << "}\"];" << endl;
    
    
    if(trueExit != NULL)
      f<<this->label<<":t ->"<<trueExit->label<<":h;"<<endl;
    if(falseExit != NULL)
      f<<this->label<<":f ->"<<falseExit->label<<":h;"<<endl;
  }


  void dump()
  {
    cout << label << endl;
    for(auto i : instructions)
      i.dump();
    if(trueExit != NULL)
      cout << "True: " << trueExit->label << endl;
    else
      cout << "True: NULL" << endl;
    if(falseExit != NULL)
      cout << "False: " << falseExit->label << endl;
    else
      cout << "False: NULL" << endl;
  }
  
  static string newName()
  {
    stringstream result;
    result << "Block" << BBlock::blockCounter++;
    return result.str();
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

  void dumpGraph(ofstream &f)
{
set<BBlock *> done, todo;
  todo.insert(this);
  while(todo.size()>0)
  {
    // Pop an arbitrary element from todo set
    auto first = todo.begin();
    BBlock *next = *first;
    todo.erase(first);

    next->blockToGraph(f);
    done.insert(next);
    if(next->trueExit!=NULL && done.find(next->trueExit)==done.end())
      todo.insert(next->trueExit);
    if(next->falseExit!=NULL && done.find(next->falseExit)==done.end())
      todo.insert(next->falseExit);
  }
}
  
  
};
#endif