#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;



class Node {
public:
    string tag, value;
    vector<Node*> children;

    Node(string t, string v) : tag(t), value(unescape(v)) {
    }
    
    Node(string t) : tag(t), value("") {
    }

    Node() {
        tag = "null";
        value = "null";
    }
    
    ~Node() {
        for(auto i : children)
            delete i;
    }
    
    string unescape(string s){
        size_t idx = s.find("\\");
        while( idx != string::npos ){
            if( idx + 1 < s.size() ){
                switch(s[idx + 1]){
                    case 'n':
                        s.replace( idx, 2, "\n");
                        break;
                    case 't':
                        s.replace( idx, 2, "\t");
                        break;
                    case '\\':
                        s.replace( idx, 2, "\\");
                        break;
                    case '\'':
                        s.replace( idx, 2, "'");
                        break;
                    case '"':
                        s.replace( idx, 2, "\"");
                        break;
                    default:
                        cout << "unexpected escape sequence"<<endl;
                        exit(1);
                }
            }
            idx = s.find("\\", idx + 1);
        }
        return s;
    }
    
    string escape(string s){
        size_t idx = s.find('"');
        while( idx != string::npos ){
            if(idx == 0 || s[idx-1] !='\\'){
                s.replace(idx, 1, "\\\"");
            }
            idx = s.find('"', idx + 1);
        }
        
        return s;
    }

    void dump(int depth = 0) {
        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << tag << ":" << value << endl;
        for(auto i : children)
            i->dump(depth + 1);
    }

    Node* add(Node* n)
    {
        this->children.push_back(n);
        return this;
    }

    int dumpToFile(ofstream &f, int &id) {
        int myID = id++;
        f << "id" << myID << " [label=\"" << tag;
        if(value!="")
            f << " -> " << escape(value);
        f << "\"];" << endl;
        for(auto i : children) {
            int childID = i->dumpToFile(f, id);
            f << "id" << myID << " -> " << "id" << childID << ";" << endl;
        }
        return myID;
    }
};

#endif  /* NODE_H */