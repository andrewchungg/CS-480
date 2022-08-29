/*
Author: Andrew Chung
RedID: 821542300
*/

#include "pagetable.h"
using namespace std;

PageTable::PageTable(){
    root = new Level();     
}

unsigned int PageTable::virtualAddressToPageNum(unsigned int virtualAddress, int index){
    unsigned temp = virtualAddress & BitmaskAry[index];
    temp = temp >> ShiftAry[index];
    return temp;
}

void PageTable::pageInsert(uint32_t *pageList, uint32_t frame) {   // pass in either levelPtr or pageTablePtr
    Level* current = root;                                      // Find index into current page level
    for (int index = 0; index < LevelCount; index++){
        if (current->children.find(pageList[index]) == current->children.end()) {
            bytesCounter[index]++;
            current->children[pageList[index]] = new Level();
        }
        current = current->children[pageList[index]];
    }
    current->map[pageList[LevelCount-1]] = frame,      // current->frame = frame;
    current->leaf = true;
}

uint32_t PageTable::search(uint32_t *pageList){
    Level* current = root;
    for (int index = 0; index < LevelCount; index++){
        if (current->children.find(pageList[index]) == current->children.end()) {
            return pow(2, MAXNUMOFBITS) - 1;
        }
        current = current->children[pageList[index]];
    }
    if (current->leaf){
        return current->map[pageList[LevelCount-1]]; // current->frame;
    }
    return pow(2, MAXNUMOFBITS) - 1;
}

Level::Level() {
    leaf = false;
    // remember to only allocate the nextLevel array for internal levels. The leaf ones use the map array
    new Level*[this->pageTablePtr->EntryCount[this->depth]];
}
