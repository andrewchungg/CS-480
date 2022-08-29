/*
Author: Andrew Chung
RedID: 821542300
*/
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <pthread.h>
#include <fstream>
#include "populatetree.h"
#include "EXEC_STATUS.h"
using namespace std;

// reading dictsrc
// do processing

struct dictNode *initNode(){
    dictNode *start = new dictNode();
    start->isWord = false;
    int iterator = 0;
    for (int i = 0; i < NUMOFCHARS; i++){
        start ->children[i] = NULL;
    }
    return start;
}

void add(struct dictNode *root, string s)
{
    if (s.length() == 0){
        return;
    }
    
    struct dictNode *current = root;

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

        if (current->children[num] == NULL)         // if current node doesnt have char
        {      
            current->children[num] = initNode();
        }
        current = current->children[num];          // points to next node
        current->iterator += 1;
    }
    current->isWord = true;
}

// pass pointer to shared structure
void *populateTree(void *arg){
    
    // cast passed-in argument to EXEC_STATUS data type
    EXEC_STATUS *thread = (EXEC_STATUS*)arg;

    ifstream file(thread->filePath[DICTSRCFILEINDEX]);

    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

    while(getline(file, line))
    {
        *thread->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] += (long)(line.size()+1);
        char * cstr = new char [line.length()+1];
        strcpy(cstr, line.c_str());
        char *word = strtok(cstr, delimiters);
        while(word != nullptr)
        {
            string wordFromFile = string(word);
            thread->wordCountInFile[DICTSRCFILEINDEX] += 1;
            add(thread->root, wordFromFile);
            word = strtok(NULL, delimiters);
        }
    }
    thread->taskCompleted[DICTSRCFILEINDEX] = true;
    file.close();
    return nullptr;
}