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
  
  static string newName()
  {
    stringstream result;
    result << "Block" << BBlock::blockCounter++;
    return result.str();
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
  
  void assembly(ofstream &f){
    f << this->label <<":"<< endl;
    bool endblock;
    for(auto i : instructions) {
      endblock = i.assembly(f);
    }
    if(endblock){
      f<<"\t"<< trueExit->label << endl;
      if(falseExit != 0){
        f <<"\tjmp\t"<< falseExit->label << endl;
      }
    }else{
      if(trueExit == 0 && falseExit == 0){
        // End of the program
        f << "\tmovq\ta,\t%rbx" << endl
          << "\tmovq\t$1,\t%rax" << endl
          << "\tint\t$0x80" << endl;
      }else{
        f <<"\tjmp\t"<< trueExit->label << endl;
      }
    }
  }
  
  void dumpAssembly(ofstream &f){
    set<BBlock *> done, todo;
    todo.insert(this);
    while(todo.size()>0)
    {
      // Pop an arbitrary element from todo set
      auto first = todo.begin();
      BBlock *next = *first;
      todo.erase(first);
  
      next->assembly(f);
      done.insert(next);
      if(next->trueExit!=NULL && done.find(next->trueExit)==done.end())
        todo.insert(next->trueExit);
      if(next->falseExit!=NULL && done.find(next->falseExit)==done.end())
        todo.insert(next->falseExit);
    }
  }
  
  
};
#endif