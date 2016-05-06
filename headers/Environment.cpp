#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include<iostream>
#include <map>
#include <vector>
using namespace std;

class Environment
{
    public:
    map<string, pair<string, string>> env;
    Environment() { }
    
    void add(string name, string type, string value){
        env[name] = make_pair(type, value);
    }
    
    bool exists(string name){
        return env.find(name) != env.end();
    }
    
    map<string, pair<string, string>> getEnv(){
        return env;
    }
    
    string getType(string var){
        return env[var].first;
    }
    
    vector<string> argtypes;
    
    void addArg(string type){
        argtypes.push_back(type);
    }
    
    int argc(){
        return argtypes.size();
    }
    
    vector<string> argTypes(){
        return argtypes;
    }
    
    void clearArgs(){
        argtypes.clear();
    }
    
    
};

#endif