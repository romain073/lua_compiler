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
    Environment() { }
    
    void add(string name, type type, string value){
        cout << name << " " << type << " " << value<<endl;
        env[name] = make_pair(type, value);
    }
    
    bool exists(string name){
        return env.find(name) != env.end();
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