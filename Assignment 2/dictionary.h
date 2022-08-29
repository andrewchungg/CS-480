#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <string>
using std::string;

class Node{
    public:
        int iterator; 
        Node *children[27];             // 27 = 26 letters + an apostrophe 
      
        Node();
};

class DictTree{
    public:
        DictTree();

        void add(string s);


        int search(string s);

    private:
        Node* root;
};
#endif