#include<iostream>
#include "compiler.tab.hh"
#include "headers/Statement.cpp"
#include "headers/Sequence.cpp"
#include "headers/Assign.cpp"
#include "headers/Constant.cpp"
#include "headers/Variable.cpp"
#include "headers/BBlock.cpp"
#include "headers/String.cpp"
#include "headers/Environment.cpp"
#include <fstream>
#include <string.h>
#include <algorithm>
extern Statement* root;
extern FILE *yyin;
void yy::parser::error(string const&err)
{
  cout << "Parse error... " << err << endl;
}

int Expression::nameCounter = 0;
int BBlock::blockCounter = 0;
int main(int argc, char **argv)
{
  yy::parser parser;
  if(argc != 2){
    cout << "Usage: ./compiler program.lua" << endl;
    return 1;
  }
  yyin = fopen(argv[1], "r");
  if(errno != 0){
    cout << argv[1] << ": " << strerror(errno) << endl;
    return 1;
  }
  
  if(!parser.parse()){
    BBlock *start = new BBlock();
    BBlock *current = start;
    
    list<BBlock*> functiondefs;
    
    root->convert(&current, functiondefs);
    
    ofstream myfile;
    myfile.open("graph.dot");
    myfile << "digraph structs { node [shape=record];" << endl;
    start->dumpGraph(myfile);
    
    for(auto i : functiondefs){
      i->dumpGraph(myfile);
    }
    
    myfile << "}" << endl;
    myfile.close(); 
    
    system("dot -Tpdf graph.dot -ograph.pdf");
    
    myfile.open("target.s");
    myfile << ".include \"lib/io.s\""<<endl;
    
  
    myfile << ".section .text"<<endl;
    myfile << ".globl _start"<<endl;
    

    for(auto i : functiondefs){
      myfile << ".type "<<i->label<<", @function"<<endl;
    }
    
    Environment env;
    for(auto i : functiondefs){
      i->dumpAssembly(myfile, env, true);
    }
    
    
    myfile << "_start:"<<endl;
    start->dumpAssembly(myfile, env);
    
    myfile << ".section .data"<<endl;
    for(auto i : env.getEnv()){
      string name(i.first), value(i.second.second);
      Environment::type type = i.second.first;
      if(type == Environment::type::STRING){
        
        int backslash_count = count(value.begin(), value.end(), '\\');
        myfile <<"\t"<< name <<":\t.quad "<< value.length()-backslash_count <<endl;
        myfile <<"\t"<< name <<"_s:\t.ascii\t\""<<value <<"\""<<endl;
      } else if(type == Environment::type::INT || 
          type == Environment::type::STRING_PTR || 
          type == Environment::type::ARRAY_PTR || 
          type == Environment::type::CELL_PTR){
        myfile <<"\t"<< name <<":\t.quad 0" <<endl;
        
      }else if(type == Environment::type::ARRAY){
        int size = count(value.begin(), value.end(), ',') + 1;
        myfile <<"\t"<< name <<":\t.quad "<< size <<endl;
        myfile <<"\t"<< name <<"_a:\t.quad "<< value <<endl;
      }
    }
    
    myfile.close();
    
    string asm_link = "as target.s -o target.o && ld target.o -o target";
    
    cout << asm_link <<endl;
    if(system(asm_link.c_str())!=0){
      cout << "An error has occured during '"<<asm_link<<"'" << endl;
      return 1;
    }
    
    start->free();
    for(auto i : functiondefs){
      i->free();
    }
    delete root;
    return 0;
  }
  return 1;
}