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
    
    myfile.open("target.s");
    myfile << ".section .data"<<endl;
    myfile<<"\t_char:\t.quad 0 # storage for printing"<<endl;
    myfile<<"\t_count:\t.quad 0 # storage for printing numbers"<<endl;
    myfile<<"\t_argc:\t.quad 0 # storage for printing numbers"<<endl;
    myfile<<"\t_idx:\t.quad 0 # storage for printing numbers"<<endl;
    
    
    myfile<<"\t_read_buf:\t.quad 0 # storage for reading numbers"<<endl;
    myfile<<"\t_bytesread:\t.quad 0 # storage for reading numbers"<<endl;
    myfile<<"\t_read_retval:\t.quad 0 # storage for reading numbers"<<endl;
    myfile<<"\t_read_mul:\t.quad 0 # storage for reading numbers"<<endl;
    myfile<<"\t_read_idx:\t.quad 0 # storage for reading numbers"<<endl;
    
    
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
    
    
      functions.push_back(make_pair("io.read", (list<string>){
"movq $8, %rdx",
"mov $_read_buf, %rcx",
"movq $0, %rbx",
"movq $3, %rax",
"int $0x80",
"movq %rax, _bytesread",
"movq $0, _read_retval",
"movq $1, _read_mul",
"movq _bytesread, %rax",
"movq %rax, _read_idx",
"decq _read_idx",
"read_beg:",
"decq _read_idx",
"movq $_read_buf, %rax",
"addq _read_idx, %rax",
"movq (%rax), %rax",
"andq $0xff, %rax",
"subq $48, %rax",
"imulq _read_mul, %rax",
"addq %rax, _read_retval",
"movq _read_mul, %rcx",
"imulq $10, %rcx",
"movq %rcx, _read_mul",
"cmpq $0, _read_idx",
"jnz read_beg",
"movq _read_retval, %rax",
"ret"
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
      string name(i.first), value(i.second.second);
      Environment::type type = i.second.first;
      if(type == Environment::type::STRING){
        myfile <<"\t"<< name <<":\t.quad "<< value.length() <<endl;
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
      //cout << name << type << value;
    }
    
    myfile.close();
    
    string asm_link = "as target.s -o target.o && ld target.o -o target";
    
    cout << asm_link <<endl;
    if(system(asm_link.c_str())==-1){
      cout << "An error has occured during '"<<asm_link<<"'" << endl;
      return 1;
    }
    // TODO clean parse tree & graphs
    return 0;
  }
  return 1;
}