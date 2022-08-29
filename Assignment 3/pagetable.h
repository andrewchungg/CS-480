#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <unordered_map>
#include <vector>
#include <climits>
using namespace std;

#define MAXNUMOFBITS 32     // max bits
#define LRU_LIMIT 10        // max size for the LRU


// PageTable struct should hold all the information about how we should populate our page table
// Each Level should point back to the PageTable and the PageTable should point to the root
// Level should be a different structure that gets created per the specifications (bits, etc) depending on which depth the new Level we are creating is
class PageTable{
    public:
        class Level* root;
        int LevelCount;
        uint32_t *bytesCounter; 
        uint32_t *BitmaskAry;
        uint32_t *ShiftAry;
        uint32_t *EntryCount; 
        PageTable();
        unsigned int virtualAddressToPageNum(unsigned int virtualAddress, int index);
        void pageInsert(uint32_t *pageList, uint32_t frame);
        uint32_t search(uint32_t *pageList);

    private:
        class Level* rootNodePtr;
};

class Level{
    public:
        bool leaf;
        unordered_map<unsigned int, Level*> children;             // contains virtual address and *PageNode (next level)
        unordered_map<uint32_t, int> map;                         // contains virtual address and frame
        int depth;
        Level();

    private:
        class PageTable* pageTablePtr;                            // could also pass this pointer back into the level constructor as argument
};

#endif