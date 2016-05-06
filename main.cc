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
      "jz return",
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
      "incq _idx",
      "movq _idx, %rax",
      "cmpq _argc, %rax",
      "jz return",
      "movq $32, %rcx",
      "call print_char",
      "jmp args",
      "return:",
      "ret"
    }));
  
      functions.push_back(make_pair("print_str", (list<string>){
      "movq 8(%rsp), %rax",
"movq %rax, _argc",
"addq $2, _argc",
"movq $2, _idx ",
"argsprintstr:",
"movq _idx, %rax",
"cmpq _argc, %rax",
"jz return",
"movq $8, %rax",
"imulq _idx",
"addq %rsp, %rax",
"movq (%rax), %rax",
"movq (%rax), %rdx",
"addq $8, %rax",
"movq %rax, %rcx",
"movq $4, %rax",
"movq $1, %rbx",
"int  $0x80",
"incq _idx",
"movq _idx, %rax",
"cmpq _argc, %rax",
"jz return",
"movq $32, %rcx",
"call print_char",
"jmp argsprintstr"
    }));
  
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
    Environment env;
    start->dumpAssembly(myfile, env);
    
    myfile << ".section .data"<<endl;
    for(auto i : env.getEnv()){
      string name(i.first), type(i.second.first), value(i.second.second);
      if(!type.compare("string")){
        
        myfile <<"\t"<< name <<":\t.quad "<< value.length() <<endl;
        myfile <<"\t"<< name <<"_s:\t.ascii\t\""<<value <<"\""<<endl;
      } else if(!type.compare("int") || !type.compare("string_ptr")){
        myfile <<"\t"<< name <<":\t.quad 0" <<endl;
        
      }else if(!type.compare("table")){
        int size = count(value.begin(), value.end(), ',') + 1;
        myfile <<"\t"<< name <<":\t.quad "<< size <<endl;
        myfile <<"\t"<< name <<"_a:\t.quad "<< value <<endl;
      }
      //cout << name << type << value;
    }
    
    myfile.close();
    system("as prog.s -o prog.o && ld prog.o -o prog && ./prog; echo \"Process exited with code: $?.\"");
    
    // TODO clean parse tree & graphs
    return 0;
  }
  return 1;
}