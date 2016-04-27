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

  void dump()
  {
    cout << result << " := " << lhs << " " 
         << op << " " << rhs << endl;
  }
  void graph(ofstream &f)
  {
    f << result << " := " << lhs << " " 
         << op << " " << rhs;
  }
};
#endif