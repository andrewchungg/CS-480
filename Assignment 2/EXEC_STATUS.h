#ifndef EXEC_STATUS_H
#define EXEC_STATUS_H

#define NUMOFTHREADS 2
#define NUMOFFILES 2
#define NUMOFCHARS 27
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1
#define DEFAULT_NUMOF_MARKS 50 // default number of progress marks for representing 100% progress 
#define MIN_NUMOF_MARKS 10 // minimum number of progress marks for representing 100% progress 
#define DEFAULT_HASHMARKINTERVAL 5 // place hash marks in the progress bar every N marks 
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX 1 // default minimum number of dictionary words starting from a prefix for printing or writing to the output 

struct dictNode{
    struct dictNode *children[NUMOFCHARS];
    bool isWord;
    int iterator;
};

/* Common data shared between threads*/
typedef struct{
    struct dictNode *root;                                    // root node of the dictionary tree
    int numOfProgressMarks;                                     // parameters for printing progress bar
    int hashmarkInterval;
    int minNumOfWordsWithAPrefixForPrinting; 
    const char *filePath[NUMOFFILES];
    long totalNumOfCharsInFile[NUMOFFILES];
    long *numOfCharsProcessedFromFile[NUMOFFILES];  
    long  wordCountInFile[NUMOFFILES]; 
    bool  taskCompleted[NUMOFFILES];  
} EXEC_STATUS;
#endif