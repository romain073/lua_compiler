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
    f<< "\tmovq\t"<<lhs<<",\t%eax"<<endl;
    if(op != 'c')
      f<< "\tmovq\t"<<rhs<<",\t%ebx"<<endl;
    
    switch(op){
      case 'c':
        //f<< "\tmv\t%eax, %ebx"<<endl;
        break;
      case '+':
        f<< "\taddq\t%eax, %ebx"<<endl;
        break;
      case 48:
        f<< "\tsubq\t%eax, %ebx"<<endl;
        break;
    }
    f<< "\tmovq\t%eax, "<<result<<endl;
  }

  
};
#endif