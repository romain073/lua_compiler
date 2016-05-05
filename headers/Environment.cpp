#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <map>
using namespace std;

class Environment
{
    public:
    map<string, pair<string, string>> env;
    Environment() { }
    
    void add(string name, string type, string value){
        env[name] = make_pair(type, value);
    }
    
    map<string, pair<string, string>> getEnv(){
        return env;
    }
    
    string getType(string var){
        return env[var].first;
    }
};

#endif