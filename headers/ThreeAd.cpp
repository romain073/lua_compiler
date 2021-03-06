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
    f<< endl<<"\t #" << result << " := " << lhs << " " 
         << op << " " << rhs << endl;
    if(op.compare("call")!=0
        && op.compare("param")!=0
        && !lhs.empty())
        env.fntranslate(lhs, f);
    if(op.compare("call")!=0
        && !rhs.empty())
        env.fntranslate(rhs, f);
    if(!result.empty())
        env.fntranslate(result, f);

    if(env.getType(lhs) == Environment::type::CELL_PTR){
      f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl; // Dereference, we want the value of lhs
      f<< "\tmovq\t(%rax),\t%rax"<<endl;
      f<< "\tmovq\t%rax,\t"<<lhs<<endl;
      env.add(lhs, Environment::type::INT, "0");
    }
    if(env.getType(rhs) == Environment::type::CELL_PTR){
      f<< "\tmovq\t"<<rhs<<",\t%rax"<<endl; // Dereference, we want the value of rhs
      f<< "\tmovq\t(%rax),\t%rax"<<endl;
      f<< "\tmovq\t%rax,\t"<<rhs<<endl;
      env.add(rhs, Environment::type::INT, "0");
    }

    if(op.compare("call")!=0
      && op.compare("assign")!=0
      && op.compare("string")!=0
      && op.compare("param")!=0
      && op.compare("table")!=0
      && !lhs.empty()){
      f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
    }
    if(op.compare("call")!=0
        && !rhs.empty())
      f<< "\tmovq\t"<<rhs<<",\t%rbx"<<endl;
      
      

      
    if(!result.empty() && !env.exists(result))
        env.add(result, Environment::type::INT, "0");
        
    if(!op.compare("assign")){
        Environment::type type = env.getType(lhs);
        if(type == Environment::type::STRING){ // Create a string pointer
            env.add(result, Environment::type::STRING_PTR, "0");
            f<< "\tmovq\t$"<<lhs<<",\t%rax"<<endl;
        } else if(type == Environment::type::ARRAY){ // Create an array pointer
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
        if(rhs.empty())
            f<< "\tnegq\t%rax"<<endl; // Unary
        else
            f<< "\tsubq\t%rbx,\t%rax"<<endl; // Binary
    } else if (!op.compare("*")){
        f   << "\timulq\t%rbx,\t%rax"<<endl;
    } else if (!op.compare("/")){
        f   << "\tcqto"<<endl
            << "\tidivq\t%rbx"<<endl;
    } else if (!op.compare("%")){
        f   << "\tcqto"<<endl
            << "\tidivq\t%rbx"<<endl
            << "\tmovq\t%rdx,\t%rax"<<endl; // put mod result in %rax
    } else if (!op.compare("#")){
        f   << "\tmovq\t(%rax), %rax"<<endl;
    } else if (!op.compare("tableaccess")){
        f   << "\timulq $8, %rbx" << endl // Compute the offset
            << "\taddq %rbx, %rax" << endl; // Move the pointer
        env.add(result, Environment::type::CELL_PTR, "0");
    } else if (!op.compare("call")){
        string args = rhs.substr(1, rhs.length()-2); // args separated by commas
        
        istringstream ss(args);
        string token;
        
        Environment::type t = Environment::type::INT;
        
        bool printFn = lhs.compare("print") ==0 || lhs.compare("io.write") == 0;
        bool print = printFn && lhs.compare("print") ==0;
        
        bool firstArg = true;
        vector<string> arguments;
        while(getline(ss, token, ',')) { // For each arg
            t = env.getType(token);
            
            if(t == Environment::type::CELL_PTR){
              f << "\tmovq\t"<<token<<",\t%rax"<<endl // Dereference, we want the value of rhs
                << "\tmovq\t(%rax),\t%rax"<<endl
                << "\tmovq\t%rax,\t"<<token<<endl;
              env.add(token, Environment::type::INT, "0");
            }
                    
            env.fntranslate(token, f);
            if(!t){
                t = Environment::type::INT;
            } 
            
            if(printFn){
                if(print && !firstArg){
                    f   << "\tcall print_tab"<<endl;
                }
                if(t == Environment::type::STRING || t == Environment::type::STRING_PTR){
                    if(t == Environment::type::STRING)
                        f   << "\tmovq $"<<token<<", %rax"<<endl;
                    else
                        f   << "\tmovq "<<token<<", %rax"<<endl;
                    f   << "\tcall print_str"<<endl;
                }else{
                    f   << "\tmovq "<<token<<", %rax"<<endl
                        << "\tcall print_nbr"<<endl;
                }
                firstArg = false;
                continue;
            }
            arguments.push_back(token);
        }
        
        if(printFn){
            if(print){
                f   << "\tcall print_nl"<<endl;
            }
            return false; // call has already been done
        }
        int i;
        int argc = arguments.size();
        for(i = argc-1;i>=0;i--){
            // Push argv
            f   << "\tpushq "<<arguments[i]<<endl;
        }
        
        
        // Push argc
        f<< "\tpushq $"<<to_string(argc)<<endl;
        

        f<< "\tcall \t"<<lhs<<endl;
        
        
        // Pop argv & argc
        f << "\taddq\t$"<< 8*(argc+1) <<",\t%rsp"<<endl;

        
    } else if (!op.compare("EQ")){
        f   << "\tsubq\t%rbx,\t%rax"<<endl
            <<"\tjz";
        return true;
    } else if (!op.compare("NE")){
        f   << "\tsubq\t%rbx,\t%rax"<<endl
            <<"\tjnz";
        return true;
    } else if (!op.compare("LT")){
        f   << "\tsubq\t%rbx,\t%rax"<<endl
            <<"\tjs";
        return true;
    } else if (!op.compare("LE")){
        f   << "\tsubq\t%rax,\t%rbx"<<endl
            <<"\tjns";
        return true;
    } else if (!op.compare("GE")){
        f   << "\tsubq\t%rbx,\t%rax"<<endl
            <<"\tjns";
        return true;
    } else if (!op.compare("GT")){
        f   << "\tsubq\t%rax,\t%rbx"<<endl
            <<"\tjs";
        return true;
    } else if (!op.compare("string")){
        env.add(result, Environment::type::STRING, lhs);
        return false;
    } else if (!op.compare("table")){
        env.add(result, Environment::type::ARRAY, lhs);
        return false;
    } else if (!op.compare("return")){
        f << "\tmovq %rbp, %rsp"<<endl // Restore %rsp & %rbp 
          <<"\tpopq %rbp"<<endl
          <<"\tret"<<endl;
    }

    if(!result.empty())
      f<< "\tmovq\t%rax,\t"<<result<<endl;
    return false;
  }

  
};
#endif