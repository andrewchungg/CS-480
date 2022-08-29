/*
Author: Andrew Chung
RedID: 821542300
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
#include <pthread.h>
#include <sched.h>
#include <cstring>
#include <cctype>
#include <unistd.h>
#include <sys/stat.h>
#include "populatetree.h"
#include "countwords.h"
#include "EXEC_STATUS.h"
using namespace std;


int main(int argc, char **argv){

    ifstream file1(argv[1]);
    ifstream file2(argv[2]);

    pthread_attr_t pthread_attributes;
    pthread_t thread1, thread2;

    struct stat file_1, file_2;

    dictNode* trie = new dictNode();
    EXEC_STATUS* specs = (EXEC_STATUS*)(malloc(sizeof(EXEC_STATUS)));

    if(!file1.good()){
        cout << "Unable to open ,," << argv[1] << ">>" << endl;
        exit(0);
    }else{
        specs->filePath[DICTSRCFILEINDEX] = argv[1];
        stat(specs->filePath[DICTSRCFILEINDEX], &file_1);
        specs->totalNumOfCharsInFile[DICTSRCFILEINDEX] = (long) file_1.st_size;
    }
    if(!file2.good()){
        cout << "Unable to open ,," << argv[2] << ">>" << endl;
        exit(0);
    }else{
        specs->filePath[TESTFILEINDEX] = argv[2];
        stat(specs->filePath[TESTFILEINDEX], &file_2);
        specs->totalNumOfCharsInFile[TESTFILEINDEX] = (long) file_2.st_size;
    }

    specs->hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    specs->numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    specs->minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;
    for (int i = 0; i < NUMOFFILES; i++){
        specs->wordCountInFile[i] = (long)0;
        specs->numOfCharsProcessedFromFile[i] = new long();
        specs->taskCompleted[i] = false;
    }

    specs->root = trie;
    int option, temp1, temp2;

    for (int i = 3; i < argc; i++ ){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'p'){
                temp1 = atoi(argv[i+1]);
                if(temp1<MIN_NUMOF_MARKS){
                    cout<< "Number of progress marks must be a number and at least 10" << endl;
                    exit(0);
                }
                specs->numOfProgressMarks = temp1;
            }
        }
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'h'){
                temp2 = atoi(argv[i+1]);
                if (temp2 <= 0 || temp2 > MIN_NUMOF_MARKS){
                    cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                    exit(0);
                }
                specs->hashmarkInterval = temp2;
            }
        }
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'n'){
                specs->minNumOfWordsWithAPrefixForPrinting = atoi(argv[i+1]);
            }
        }
    }

    
    pthread_create(&thread1, &pthread_attributes, &populateTree, (void *)specs);
    pthread_create(&thread2, NULL, &countWords, (void *)specs);

    double holder = (double) 1 / specs->numOfProgressMarks;
    int inc = 0;
    int count = 0;
    int marks;

    while (!specs->taskCompleted[DICTSRCFILEINDEX]){
        marks = ((double)* specs->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] / (double) specs->totalNumOfCharsInFile[DICTSRCFILEINDEX]) / holder;
        for (int i = 0; i < marks - count; i++){
            inc += 1;
            if (inc % specs->hashmarkInterval == 0){
                cout << "#";
            }
            else {
                cout << "-";
            }
        }
        count = marks;
    }
     
    cout << '\n';
    cout << "There are " << specs->wordCountInFile[DICTSRCFILEINDEX] << " words in " << specs->filePath[DICTSRCFILEINDEX] << "." << '\n';

    while(!specs->taskCompleted[TESTFILEINDEX]){             
        marks = ((double)* specs->numOfCharsProcessedFromFile[TESTFILEINDEX] / (double) specs->totalNumOfCharsInFile[TESTFILEINDEX]) / holder;
        for (int i = 0; i < marks - count; i++){
            inc += 1;
            if (inc % specs->hashmarkInterval == 0){
                cout << "#";
            }
            else {
                cout << "-";
            }
        }
        count = marks;
    }

    cout << '\n';
    cout << "There are " << specs->wordCountInFile[TESTFILEINDEX] << " words in " << specs->filePath[TESTFILEINDEX] << "." << '\n';

    pthread_exit(NULL);
}
