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
#include <unistd.h>
#include <fstream>
#include "countwords.h"
#include "populatetree.h"
#include "EXEC_STATUS.h"
using namespace std;

// reading testfile
// do processing

int search(struct dictNode *root, string s)
{
    if(s.length() == 0)
    {
        return false;
    }

    struct dictNode *current = root;

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

// pass pointer to shared structure
void *countWords(void *arg){
    // cast passed-in argument to EXEC_STATUS data type
    EXEC_STATUS *thread = (EXEC_STATUS*)arg;
    string line;
    const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

    while(thread->taskCompleted[TESTFILEINDEX] != true){
        if (thread->taskCompleted[DICTSRCFILEINDEX] == true){
            ifstream file2(thread->filePath[TESTFILEINDEX]);
            ofstream outputFile("countwords_output.txt");

            while(getline(file2, line)){
                *thread->numOfCharsProcessedFromFile[TESTFILEINDEX] += (long)(line.size()+1);
                char * cstr = new char [line.length()+1];
                strcpy(cstr, line.c_str());
                char *word = strtok(cstr, delimiters);

                while(word != nullptr)
                {
                    string wordFromFile = string(word);
                    int iterator = search(thread->root, wordFromFile);
                    if(iterator >= thread->minNumOfWordsWithAPrefixForPrinting){
                        outputFile << wordFromFile << ' ' << iterator << endl;
                    }
                    thread->wordCountInFile[TESTFILEINDEX] += (long)1;
                    word = strtok(NULL, delimiters);
                }
            }
            file2.close();
            outputFile.close();
            thread->taskCompleted[TESTFILEINDEX] = true;
            }
        }
    
    return nullptr;
}
