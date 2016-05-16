#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include<iostream>
#include <map>
#include <vector>
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
    
    void addParam(string name){
        params.push_back(name);
    }
    
    string translateParam(string s){
        int c = 0;
        for(string i : params){
            if(i.compare(s)==0){
                return to_string((c+3)*8)+"(%rbp)"; // offset is 3 because of the old value of rbp & argc
            }
            c++;
        }
        return s;
    }
    
    map<string, pair<type, string>> getEnv(){
        return env;
    }
    
    type getType(string var){
        return env[var].first;
    }
    
    vector<type> argtypes;
    
    void addArg(type type){
        argtypes.push_back(type);
    }
    
    int argc(){
        return argtypes.size();
    }
    
    vector<type> argTypes(){
        return argtypes;
    }
    
    void clearArgs(){
        argtypes.clear();
    }
    
    
};

#endif