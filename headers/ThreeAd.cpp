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
    if(op.compare("call")!=0
      && op.compare("argv")!=0
      && op.compare("print")!=0
      && op.compare("c")!=0
      && op.compare("string")!=0
      && op.compare("table")!=0
      && !lhs.empty()){
      f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
    }
    if(op.compare("c")!=0 && !rhs.empty())
      f<< "\tmovq\t"<<rhs<<",\t%rbx"<<endl;
      
    if(!result.empty() && !env.exists(result))
        env.add(result, "int", "0");
        
    if(!op.compare("c")){
        string type = env.getType(rhs);
        if(!type.compare("string")){
            env.add(result, "string_ptr", "0");
            f<< "\tmovq\t$"<<lhs<<",\t%rax"<<endl;
        } else if(!type.compare("table")){
            env.add(result, "table_ptr", "0");
            f<< "\tmovq\t$"<<lhs<<",\t%rax"<<endl;
        } else {
            if(!env.exists(result))
                env.add(result, type, "0"); // Override the type with the one of the rhs
            f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
        }
        //f<< "\tmv\t%rax,\t%rbx"<<endl;
        
    } else if (!op.compare("+")){
        f<< "\taddq\t%rbx,\t%rax"<<endl;
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
    } else if (!op.compare("tableaccess")){
        f << "\t#tableaccess"<<endl;
        f << "movq "<<rhs<<", %rcx"<<endl
        << "incq %rcx" << endl
        << "movq "<<lhs<<", %rax" << endl
        << "imulq $8, %rcx" << endl
        << "addq %rcx, %rax" << endl
        << "movq (%rax), %rax"<<endl;
	
	
	
        
        
    } else if (!op.compare("call")){
        int argc = env.argc();
        
        vector<string> types = env.argTypes();
        f << "\t#"<<lhs<<"(";
        for(auto i : types){
            f<<i<<" ";
        }
        f<<")"<<endl;
        
        // Push argc
        f<< "\tpushq $"<<to_string(argc)<<endl;
        
        if(lhs == "print"){
            if(types[0] == "int"){
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
        string type = env.getType(lhs);
        if(!type.compare("string")){
            f   << "movq $4, %rax"<<endl
            << "movq $1, %rbx" << endl
            << "movq "<< lhs <<", %rdx" << endl
            << "movq $"<< lhs <<"_s, %rcx" << endl
            << "int  $0x80" << endl;
        } else if(!type.compare("string_ptr")){
            env.add(result, "string_ptr", "0");
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
        f   << "movq $4, %rax"<<endl
            << "movq $1, %rbx" << endl
            << "movq $1, %rdx" << endl
            << "movq $10, _char" << endl
            << "movq $_char, %rcx" << endl
            << "int  $0x80" << endl;
    } else if ( !op.compare("argv")){
        string type(env.getType(lhs));
        if(!type.compare("")){
            type = "int";
        } 
        if(!type.compare("string")){
            type="string_ptr";
            
            f   << "push $"<<lhs<<endl;
        }else{
            
            f   << "push "<<lhs<<endl;
        }
        
        env.addArg(type);
    } else if (!op.compare("string")){
        env.add(result, "string", lhs);
        return false;
    } else if (!op.compare("table")){
        env.add(result, "table", lhs);
        return false;
    }

    if(!result.empty())
      f<< "\tmovq\t%rax,\t"<<result<<endl;
    return false;
  }

  
};
#endif