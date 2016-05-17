#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include<iostream>
#include <map>
#include <vector>
#include <fstream>
using namespace std;

class Environment
{
    public:
    enum type {NONE, INT, STRING, STRING_PTR, ARRAY, ARRAY_PTR, CELL_PTR};
    map<string, pair<type, string>> env;
    bool function;
    Environment(bool function = false):function(function){ }
    
    void add(string name, type type, string value){
        env[name] = make_pair(type, value);
    }
    
    bool exists(string name){
        return env.find(name) != env.end();
    }
    
    bool isFunction(){
        return function;
    }
    
    vector<string> params;
    vector<string> locals;
    
    void addParam(string name){
        params.push_back(name);
    }
    
    bool fntranslate(string &s, ofstream &f){
        if(!isFunction() || s[0]=='$')
            return false;
        int c = 0;
        for(string i : params){
            if(i.compare(s)==0){
                s = to_string((c+3)*8)+"(%rbp)"; // offset is 3 because of the old value of rbp & argc
                return false;
            }
            c++;
        }
        
            
        c = 0;
        for(string i : locals){
            if(i.compare(s)==0){
                s = "-"+to_string((c+1)*8)+"(%rbp)";
                return false;
            }
            c++;
        }
        
        locals.push_back(s);
        f<< "\tsubq $8, %rsp\t#Allocate local variable "<<s<<endl;
        s = "-"+to_string((c+1)*8)+"(%rbp)";
        
        
        return true;
    }
    
    map<string, pair<type, string>> getEnv(){
        return env;
    }
    
    type getType(string var){
        return env[var].first;
    }
};

#endif