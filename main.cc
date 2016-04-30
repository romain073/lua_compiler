#include<iostream>
#include "compiler.tab.hh"
#include "headers/Statement.cpp"
#include "headers/Sequence.cpp"
#include "headers/Assign.cpp"
#include "headers/Constant.cpp"
#include "headers/Variable.cpp"
#include "headers/BBlock.cpp"
#include "headers/String.cpp"
#include <fstream>
#include <string.h>
extern Statement* root;
extern FILE *yyin;
void yy::parser::error(string const&err)
{
  cout << "Parse error... " << err << endl;
}

int Expression::nameCounter = 0;
set<string> Expression::names;
vector<pair<string,string>> String::strings;
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
    //root->dump();
    
    BBlock *start = new BBlock();
    BBlock *current = start;
    
    root->convert(&current);
    
    ofstream myfile;
    myfile.open("graph.dot");
    myfile << "digraph structs { node [shape=record];" << endl;
    start->dumpGraph(myfile);
    myfile << "}" << endl;
    myfile.close(); 
    
    system("dot -Tpdf graph.dot -ograph.pdf");
    
    myfile.open("prog.s");
    myfile << ".section .data"<<endl;
    myfile<<"\t_char:\t.quad 0 # storage for printing"<<endl;
    myfile<<"\t_count:\t.quad 0 # storage for printing numbers"<<endl;
    myfile<<"\t_argc:\t.quad 0 # storage for printing numbers"<<endl;
    myfile<<"\t_idx:\t.quad 0 # storage for printing numbers"<<endl;
    for(string s : Expression::names){
      myfile<<"\t"<<s<<":\t.quad 0"<<endl;
    }
    
    
    vector<pair<string, list<string>>> functions;

    functions.push_back(make_pair("print_char", (list<string>){
      "movq $4, %rax",
      "movq $1, %rbx",
      "movq %rcx, _char",
      "movq $1, %rdx",
      "movq $_char, %rcx",
      "int  $0x80",
      "ret"
    }));
    
    functions.push_back(make_pair("print_nbr", (list<string>){
      "movq 8(%rsp), %rax",
      "movq %rax, _argc",
      "addq $2, _argc",
      "movq $2, _idx ",
      "args:",
      "movq _idx, %rax",
      "cmpq _argc, %rax",
      "jnz p",
      "movq $10, %rcx",
      "call print_char",
      "ret",
      "p:",
      "movq $8, %rax",
      "imulq _idx ",
      "addq %rsp, %rax ",
      "movq (%rax), %rax ",
      "movq $0, _count ",
      "loop:",
      "incq _count ",
      "movq $0, %rdx",
      "movq $10, %rbx",
      "idivq %rbx",
      "addq $48, %rdx",
      "pushq %rdx",
      "cmpq $0, %rax",
      "jnz loop",
      "prloop:",
      "decq _count",
      "popq %rcx",
      "call print_char",
      "cmpq $0, _count",
      "jnz prloop",
      "movq $32, %rcx",
      "call print_char",
      "incq _idx",
      "jmp args"
    }));
  
    for(auto s : String::strings){
      myfile <<"\t"<< s.first <<":\t.ascii\t\""<<s.second <<"\""<<endl;
      myfile <<"\t"<< s.first <<"_len = \t. - "<< s.first<<endl;

    }

    myfile << ".section .text"<<endl;
    myfile << ".globl _start"<<endl;
    
    for(auto s : functions){
      myfile << ".type "<<s.first<<", @function"<<endl;
    }
    for(auto s : functions){
      myfile <<s.first<<":"<<endl;
      for(auto i : s.second)
        myfile<<"\t"<<i<<endl;
    }
    
    myfile << "_start:"<<endl;
    
    start->dumpAssembly(myfile);
    myfile.close();
    
    system("as prog.s -o prog.o && ld prog.o -o prog && ./prog; echo \"Process exited with code: $?.\"");
    
    // TODO clean parse tree & graphs
    return 0;
  }
  return 1;
}