#ifndef THREEAD_H
#define THREEAD_H

#include <string>
#include<iostream>
#include<sstream>
#include <fstream>
#include "Environment.cpp"
using namespace std;

class ThreeAd
{
public:
  string result;
  string op;
  string lhs, rhs;

  ThreeAd(string out, string o, string l, string r)
    :  result(out), op(o), lhs(l), rhs(r)      {}

  void graph(ofstream &f)
  {
    f << result << " := " << lhs << " " 
         << op << " " << rhs;
  }
  
  // Returns true if the last instruction is the beginning of a jump
  bool assembly(ofstream &f, Environment &env){
    //cout << "'"<<op<<"'"<<endl;
    f<< endl<<"\t #" << result << " := " << lhs << " " 
         << op << " " << rhs << endl;
    lhs = env.translateParam(lhs);
    rhs = env.translateParam(rhs);

    if(env.getType(lhs) == Environment::type::CELL_PTR){
      f<< "\tmovq\t("<<lhs<<"),\t%rax"<<endl; // Dereference, we want the value of lhs
      f<< "\tmovq\t(%rax),\t%rax"<<endl;
      f<< "\tmovq\t%rax,\t"<<lhs<<endl;
      env.add(lhs, Environment::type::INT, "0");
    }
    if(env.getType(rhs) == Environment::type::CELL_PTR){
      f<< "\tmovq\t("<<rhs<<"),\t%rax"<<endl; // Dereference, we want the value of rhs
      f<< "\tmovq\t(%rax),\t%rax"<<endl;
      f<< "\tmovq\t%rax,\t"<<rhs<<endl;
      env.add(rhs, Environment::type::INT, "0");
    }

    if(op.compare("call")!=0
      && op.compare("print")!=0
      && op.compare("c")!=0
      && op.compare("string")!=0
      && op.compare("param")!=0
      && op.compare("table")!=0
      && !lhs.empty()){
      f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
    }
    if(!rhs.empty())
      f<< "\tmovq\t"<<rhs<<",\t%rbx"<<endl;
      
      

      
    if(!result.empty() && !env.exists(result))
        env.add(result, Environment::type::INT, "0");
        
    if(!op.compare("c")){
        Environment::type type = env.getType(lhs);
        if(type == Environment::type::STRING){
            env.add(result, Environment::type::STRING_PTR, "0");
            f<< "\tmovq\t$"<<lhs<<",\t%rax"<<endl;
        } else if(type == Environment::type::ARRAY){
            env.add(result, Environment::type::ARRAY_PTR, "0");
            f<< "\tmovq\t$"<<lhs<<",\t%rax"<<endl;
        } else {
            if(!env.exists(result))
                env.add(result, type, "0"); // Override the type with the one of the lhs
            f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
        }
        if(env.getType(result) == Environment::type::CELL_PTR){
            f<< "\tmovq\t"<<result<<",\t%rcx"<<endl;
            f<< "\tmovq\t%rax,\t(%rcx)"<<endl; // Move the result to the pointed cell
            return false;
        }
        
    } else if (!op.compare("+")){
        f<< "\taddq\t%rbx,\t%rax"<<endl;
    } else if (!op.compare("param")){
        
        env.addParam(lhs);
        
    } else if (!op.compare("-")){
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
    } else if (!op.compare("*")){
        f<< "\timulq\t%rbx,\t%rax"<<endl;
    } else if (!op.compare("/")){
        f << "\tcqto"<<endl;
        f<< "\tidivq\t%rbx"<<endl;
    } else if (!op.compare("%")){
        f << "\tcqto"<<endl;
        f<< "\tidivq\t%rbx"<<endl;
        f<< "\tmovq\t%rdx,\t%rax"<<endl;
    } else if (!op.compare("#")){
        f << "\tmovq\t(%rax), %rax"<<endl;
    } else if (!op.compare("tableaccess")){
        f << "\timulq $8, %rbx" << endl
        << "\taddq %rbx, %rax" << endl;
        env.add(result, Environment::type::CELL_PTR, "0");
    } else if (!op.compare("call")){
        int argc = env.argc();
        
        vector<Environment::type> types = env.argTypes();
        f << "\t#"<<lhs<<"(";
        for(auto i : types){
            f<<i<<" ";
        }
        f<<")"<<endl;
        
        // Push argc
        f<< "\tpushq $"<<to_string(argc)<<endl;
        
        if(lhs == "print"){
            if(types[0] == Environment::type::INT){
                f<< "\tcall \tprint_nbr"<<endl;
            } else {
                f<< "\tcall \tprint_str"<<endl;
            }
        }else {
            f<< "\tcall \t"<<lhs<<endl;
        }
        
        
        env.clearArgs();
        // Pop argv & argc
        f << "\taddq\t$"<< 8*(argc+1) <<",\t%rsp"<<endl;
        
        
    } else if (!op.compare("EQ")){
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
        f<<"\tjz";
        return true;
    } else if (!op.compare("NE")){
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
        f<<"\tjnz";
        return true;
    } else if (!op.compare("LT")){
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
        f<<"\tjs";
        return true;
    } else if (!op.compare("LE")){
        f<< "\tsubq\t%rax,\t%rbx"<<endl;
        f<<"\tjns";
        return true;
    } else if (!op.compare("GE")){
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
        f<<"\tjns";
        return true;
    } else if (!op.compare("GT")){
        f<< "\tsubq\t%rax,\t%rbx"<<endl;
        f<<"\tjs";
        return true;
    } else if (!op.compare("print")){
        Environment::type type = env.getType(lhs);
        if(type==Environment::type::STRING){
            f   << "movq $4, %rax"<<endl
            << "movq $1, %rbx" << endl
            << "movq "<< lhs <<", %rdx" << endl
            << "movq $"<< lhs <<"_s, %rcx" << endl
            << "int  $0x80" << endl;
        } else if(type==Environment::type::STRING_PTR){
            env.add(result, Environment::type::STRING_PTR, "0");
            f  << "movq "<< lhs <<", %rax" << endl
            << "movq (%rax), %rdx" << endl
            << "movq 8(%rax), %rcx" << endl
            << "movq $4, %rax"<<endl
            << "movq $1, %rbx" << endl
            << "int  $0x80" << endl;
        } else {
            cout <<"print unknown type"<<endl;
            exit(42);
        }

    } else if (!op.compare("print_nl")){
        f   << "\tmovq $4, %rax"<<endl
            << "\tmovq $1, %rbx" << endl
            << "\tmovq $1, %rdx" << endl
            << "\tmovq $10, _char" << endl
            << "\tmovq $_char, %rcx" << endl
            << "\tint  $0x80" << endl;
    } else if ( !op.compare("argv")){
        Environment::type type = env.getType(lhs);
        if(!type){
            type = Environment::type::INT;
        } 
        if(type == Environment::type::STRING){
            type=Environment::type::STRING_PTR;
            f   << "\tpushq $"<<lhs<<endl;
        }else{
            f   << "\tpushq %rax"<<endl;
        }
        
        env.addArg(type);
    } else if (!op.compare("string")){
        env.add(result, Environment::type::STRING, lhs);
        return false;
    } else if (!op.compare("table")){
        env.add(result, Environment::type::ARRAY, lhs);
        return false;
    }

    if(!result.empty() && !env.isFunction())
      f<< "\tmovq\t%rax,\t"<<result<<endl;
    return false;
  }

  
};
#endif