#ifndef THREEAD_H
#define THREEAD_H

#include <string>
#include<iostream>
#include<sstream>
#include <fstream>
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
  bool assembly(ofstream &f){
    //cout << "'"<<op<<"'"<<endl;
    f<< endl<<"\t #" << result << " := " << lhs << " " 
         << op << " " << rhs << endl;
    if(op.compare("x")!=0
      && op.compare("push")!=0
      && !lhs.empty()){
      f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
    }
    if(op.compare("c")!=0 && !rhs.empty())
      f<< "\tmovq\t"<<rhs<<",\t%rbx"<<endl;
    
    if(!op.compare("c")){
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
    } else if (!op.compare("x")){
        f<< "\t#call \t"<<lhs<<endl;
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
    }

    if(!result.empty())
      f<< "\tmovq\t%rax,\t"<<result<<endl;
      return false;
  }

  
};
#endif