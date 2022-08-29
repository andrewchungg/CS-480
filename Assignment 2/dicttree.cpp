#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include "dicttree.h"
using namespace std;

class Node{
    public:
        int iterator; 
        Node *children[27];             // 27 = 26 letters + an apostrophe 
        Node() 
        {
            // iterator is at 0 
            iterator = 0;
            for (int index = 0; index < 27; index++)
            {
                children[index] = NULL;
            }
        }
};

class DictTree{
    public:
        DictTree(){
            root = new Node();
        }

        void add(string s)
        {
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


        int search(string s)
        {
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

    private:
        Node* root;
};