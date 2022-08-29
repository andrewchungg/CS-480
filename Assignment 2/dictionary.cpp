#include <iostream>
#include "dictionary.h"
using namespace std;

Node::Node(){
    // iterator is at 0 
    iterator = 0;
    for (int index = 0; index < 27; index++)
    {
        children[index] = NULL;
    }
}

DictTree::DictTree(){
    root = new Node();
}

void DictTree::add(string s){
   if (s.length() == 0){
                return;
            }
            Node* current = root;
            for (int index = 0; index < s.length(); index++){
                int num;
                char curChar = tolower(s[index]);        // make current character lowercase

                if (curChar == '\'')
                {
                    num = 26;
                }
                else 
                {
                    num = curChar - 'a';      // finding ascii value
                }

                if (current->children[num] == NULL)
                {      // if current node doesnt have char
                    current->children[num] = new Node();
                }
                current = current->children[num];          // points to next node
                current->iterator += 1;
            } 
}

int DictTree::search(string s){
    if(s.length() == 0)
    {
        return false;
    }
    Node* current = root;
    for (int index = 0; index < s.length(); index++)
    {
        int num;
        char curChar = tolower(s[index]);
        if (curChar == '\'')
        {
            num = 26;
        }
        else 
        {
            num = curChar - 'a';      // finding ascii value
        }

        if (current->children[num] == NULL) 
        {
            return 0;     // return 0 if current char is not in children of cur node
        }
        current = current->children[num];
    } 
    return current->iterator;      // returns number of words in trie with this prefix
}