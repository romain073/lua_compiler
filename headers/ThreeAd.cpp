#ifndef THREEAD_H
#define THREEAD_H

#include <string>
#include<iostream>
#include<sstream>
using namespace std;

class ThreeAd
{
public:
  string result;
  char op;
  string lhs, rhs;

  ThreeAd(string out, char o, string l, string r)
    :  result(out), op(o), lhs(l), rhs(r)      {}

  void dump()
  {
    cout << result << " := " << lhs << " " 
         << op << " " << rhs << endl;
  }
  
  void assembly(){
    switch(op){
      case '+':
        cout << "movq " << lhs << ", %rax" << "# Addition begin" << endl;
        cout << "movq " << rhs << ", %rbx" << endl;
        cout << "addq %rax, %rbx" << endl;
        cout << "movq %rbx, " << result << "# Addition end"<< endl;
        break;
      case '*':
        cout << "movq " << lhs << ", %rax" << "# Multiplication begin" << endl;
        cout << "movq " << rhs << ", %rbx" << endl;
        cout << "mulq %rbx" << endl;
        cout << "movq %rax, " << result << "# Multiplication end"<< endl;
        break;
      case 'c':
        cout << "movq "<< rhs <<", %rax# Affectation"<< endl;
        cout << "movq %rax,"  << result << "# Affectation"<< endl;
        break;
      case '=':
        cout << "movq "<< rhs <<", %rax # Comparison begin" << endl;
        cout << "movq "<< lhs <<", %rbx" << endl;
        cout << "subq %rax, %rbx" << endl;
        cout << "jz ";
        break;
      default:
        cout << "Not implemented "<< result << " := " << lhs << " " 
         << op << " " << rhs << endl;
    }
  }
};
#endif