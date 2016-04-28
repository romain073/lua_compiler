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
  char op;
  string lhs, rhs;

  ThreeAd(string out, char o, string l, string r)
    :  result(out), op(o), lhs(l), rhs(r)      {}

  void graph(ofstream &f)
  {
    f << result << " := " << lhs << " " 
         << op << " " << rhs;
  }
  
  void assembly(ofstream &f){
    f<< "\tmovq\t"<<lhs<<",\t%rax"<<endl;
    if(op != 'c')
      f<< "\tmovq\t"<<rhs<<",\t%rbx"<<endl;
    
    switch(op){
      case 'c':
        //f<< "\tmv\t%rax,\t%rbx"<<endl;
        break;
      case '+':
        f<< "\taddq\t%rbx,\t%rax"<<endl;
        break;
      case '*':
        f<< "\timulq\t%rbx,\t%rax"<<endl;
        break;
      case '/':
        f << "\tcqto"<<endl;
        f<< "\tidivq\t%rbx"<<endl;
        break;
      case '-':
      case 48:
        f<< "\tsubq\t%rbx,\t%rax"<<endl;
        break;
    }
    f<< "\tmovq\t%rax,\t"<<result<<endl;
  }

  
};
#endif