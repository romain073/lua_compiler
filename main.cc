#include<iostream>
#include "compiler.tab.hh"
#include "headers/Node.cpp"
#include "headers/Statement.cpp"
#include "headers/Sequence.cpp"
#include "headers/Assign.cpp"
#include "headers/Constant.cpp"
#include "headers/Variable.cpp"
#include <fstream>
#include <string.h>
extern Statement* root;
extern FILE *yyin;
void yy::parser::error(string const&err)
{
  cout << "Parse error... " << err << endl;
}

int main(int argc, char **argv)
{
  yy::parser parser;
  if(argc != 2){
    cout << "Usage: ./int program.lua" << endl;
    return 1;
  }
  yyin = fopen(argv[1], "r");
  if(errno != 0){
    cout << argv[1] << ": " << strerror(errno) << endl;
    return 1;
  }
  
  if(!parser.parse()){
    root->dump();
    /*ofstream myfile;
    myfile.open("tree.dot");
    int id = 0;
    myfile << "digraph {" << endl;
    root->dumpToFile(myfile, id);
    myfile << "}" << endl;
    myfile.close();
    delete root;*/
    return 0;
  }
  return 1;
}