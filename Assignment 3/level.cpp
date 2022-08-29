/*
Author: Andrew Chung
RedID: 821542300
*/

#include "level.h"

// set depth to 0, have it point back to the PageTable, allocate an array of 256 (2^8) pointers to Level structures
// - initialize all to NULL (number of level 1 entries)
// - if this had been a 1 level page table we would have allocated map structures instead of pointers to Levels
class Level{
    public:

        bool leaf;
        uint32_t frame;

        Level() {
            int depth = 0; 
            nextLevel = new *pointers[EntryCount[depth]];

        }

        Node() {
            // iterator is at 0 
            iterator = 0;
            for (int index = 0; index < 27; index++){
                children[index] = NULL;
            }
        }

    private:
        class PageTable* pageTablePtr;
};
