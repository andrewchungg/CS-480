/*
Author: Andrew Chung
RedID: 821542300
*/

#include "pagetable.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

using namespace std;

int main(int argc, char **argv){

    int maxMemoryAccess = INT_MAX;      // default is toprocess all addresses if not present
    int TLBCacheCapacity = 0;           // default cache capacity = 0 -> no TLB caching
    string outputMode = "summary";      // default output mode
    int opt;
    int cacheFlag = false;

    while ((opt = getopt(argc, argv, "n:c:o:")) != -1) 
    {
        switch (opt) {
            case 'n':
                // max memory access
                maxMemoryAccess = atoi(argv[optind]);
                break;
            case 'c':
                // TLBCacheCapacity
                if (argv[optind >= 0]){
                    TLBCacheCapacity = atoi(argv[optind]);
                    cacheFlag = true;
                }
                else {
                    cout << "Cache capacity must be a number, greater than or equal to 0" << endl;
                    exit(0);
                }
                break;
            case 'o':
                // Output mode
                outputMode = argv[optind];
                break;
        } 
    }

    FILE* tracefile = fopen(argv[optind], "rb");                    /* trace file */
    // pass tracefile omtp tracereader function NextAddress()

    /* attempt to open trace file */
    if ((tracefile = fopen(argv[optind],"rb")) == NULL) {
        cout << "Unable to open <<trace.tr>>" << endl;
        exit(1);
    }

    uint32_t *levelArray = new uint32_t[argc - (optind+1)]; 
    // this just allocates memory for the pointer, not allocating memory for the actual heap
    // don't know how big it needs to be until after the loop
    // maybe use vector instead of an array to not reallocate larger sizes as levels and level amounts grow
    // vector allows you to set LevelCOunt ot the size of the vector
    int numOfBits = 0;
    int temp;

    // loop for Level arguments
    for (int i = 0; i < argc; i++ ){
        temp = atoi(argv[optind + 1 + i]);
        levelArray[i] = temp;
        numOfBits += temp;                              // total number of bits
        if (numOfBits < 1){
            cout << "Level 0 page table must be at least 1 bit" << endl;
            exit(0);
        } else if (numOfBits > 28){
            cout << "Too many bits used in page tables" << endl;
            exit(0);
        }
    }
    
    PageTable* pageTable = new PageTable();     
    // need to use arrow operator instead of dot to dereference the pointer and grab member
    // Otherwise, would have to do (*pageTable).EntryCount[i]
    // Best way is with the pointer and the -> operator, puts less stuff on the stack

     // initialize EntryCount, BitMask, and BitShift as arrays of size levelArray
    pageTable->LevelCount = argc - (optind +1);
    pageTable->bytesCounter = new uint32_t[pageTable->LevelCount];
    pageTable->BitmaskAry = new uint32_t[pageTable->LevelCount];
    pageTable->ShiftAry = new uint32_t[pageTable->LevelCount];
    pageTable->EntryCount = new uint32_t[pageTable->LevelCount];
    /*
    declaring the arrays as pointers, and then allocating the memory for them at runtime. 
    It’s the way to get around not knowing how large they need to be at compile time
    new and malloc return pointers to blocks of memory in the heap
    */

    int pos = MAXNUMOFBITS - numOfBits;
    int numOfBitsToShift = numOfBits;

    // populate EntryCount[], ShiftAry[], and BitmaskAry[] 
    for (int i = 0; i < pageTable->LevelCount; i++ ){
        pageTable->EntryCount[i] = 1 << levelArray[i]; 
        pageTable->ShiftAry[i] = MAXNUMOFBITS - numOfBitsToShift - levelArray[i];

        unsigned int mask = pow(2, levelArray[i]) - 1;      // calculate mask in decimal with (2^length) - 1
        pageTable->BitmaskAry[i] =  mask << (pos - levelArray[i]);     // shift the mask to the correct position to add to array 
        numOfBitsToShift += levelArray[i];                         // update number of bits
        pos -= levelArray[i];                               // update position
    }

    unsigned long i = 0;  /* instructions processed */
    p2AddrTr trace;	        /* traced address */
    uint32_t vpnAddr;
	
    uint32_t *pageList = new uint32_t[argc - (optind+1)]; 
    uint32_t vpn;
    int count = 0;
    int cacheHitCount = 0;                                          
    int frameCount = 0;
    int pageHits = 0;

    int offsetMask;                             // temp offset for the different offset for each address
    uint32_t offsetSize = 32 - numOfBits;
    uint32_t offsetMask1 = (1 << (32-numOfBits)) - 1;

    unordered_map<uint32_t, int> TLB;          // initialize TLB hashmap, key = vpn, element = frame number
    unordered_map<uint32_t, int> LRU;          // initialize LRU hashmap, ley = vpn, element = access time
    
    if (outputMode == "bitmasks"){
        report_bitmasks(pageTable->LevelCount, pageTable->BitmaskAry);
        exit(0);
    }

    /* grab and store virtual addresses from .tr file */
    while (!feof(tracefile)) {
        /* get next address and process */
        if (NextAddress(tracefile, &trace)) {
            if (count > maxMemoryAccess){           // || cacheHitCount > TLBCacheCapacity
                break;          // break if number of addresses processed > maxMemoryAddress or cacheHitCount > TLBCacheCapacity
            }

            int currentFrame;
            vpnAddr = trace.addr;

            if (outputMode == "offset"){
                hexnum(vpnAddr & offsetMask1);
                continue;
            }

            vpn = pageTable->virtualAddressToPageNum(vpnAddr, i);

            // break up vpn into pieces
            for (int i = 0; i < pageTable->LevelCount; i++){
                pageList[i] = pageTable->virtualAddressToPageNum(vpnAddr, i);
            }

            if (cacheFlag){
                // if in TLB = cache hits
                if (!(TLB.find(vpn) == TLB.end())){  
                    // if LRU[vpn] doesnt exist in LRU because of past deletes
                    if (LRU.find(vpn) == LRU.end()){ 
                        // iterate to find smallest acccess time and delete it
                        int smallestAccessTime = INT_MAX;
                        unsigned int virtualKey;
                        std::unordered_map<unsigned int, int>::iterator it = LRU.begin();
                        while (it != LRU.end()){
                            if (smallestAccessTime > it->second){
                                smallestAccessTime = it->second;
                                virtualKey = it->first;
                            }
                            it++;
                        }
                        LRU.erase(virtualKey);
                        LRU[vpn] = count;               // LRU[vpn] gets the new access time
                    }
                    //if LRU[vpn] does have a value, update LRU
                    else{
                        LRU[vpn] = count;               
                    }
                    cacheHitCount++;                                                                    // increment cache hit
                }
                // if not in TLB = cache misses
                else{
                    // if in page table
                    currentFrame = pageTable->search(pageList);
                    if (currentFrame != pow(2, 32) - 1){
                        pageHits++;
                    }
                    // if not in page table
                    else{
                        // insert address
                        pageTable->pageInsert(pageList, frameCount);   
                        currentFrame = frameCount;   
                        frameCount++;                               
                    }
                    // iterate to find smallest access time and delete it from LRU and TLB   
                    int smallestAccessTime = INT_MAX;
                    unsigned int virtualKey;
                    unordered_map<unsigned int, int>::iterator it = LRU.begin();   // declare iterator
                    while (it != LRU.end()){
                        if (smallestAccessTime > it->second){
                            smallestAccessTime = it->second;
                            virtualKey = it->first;
                        }
                        it++;
                    }
                    // if TLB is full
                    if (TLB.size() == TLBCacheCapacity){
                        TLB.erase(virtualKey);
                        // LRU and TLB get new frame number and access time
                        LRU.erase(virtualKey);
                        LRU[vpn] = count;
                        TLB[vpn] = pageTable->search(pageList);                                      
                    }
                    // if TLB is not full but LRU is full  
                    else if (LRU.size() == LRU_LIMIT){
                        // LRU and TLB get new frame number and access time
                        LRU.erase(virtualKey);
                        LRU[vpn] = count;
                        TLB[vpn] = pageTable->search(pageList);                                  
                    }
                    // else (TLB and LRU are not full)
                    else{
                        // enter in access time and frame number
                        LRU[vpn] = pageTable->search(pageList);
                        TLB[vpn] = count;
                    }
                    offsetMask = offsetMask + ((TLB[vpn]) * pow(2, (MAXNUMOFBITS - numOfBits)));        // prep offset mask to print  
                }
                if (outputMode == "v2p_tlb_pt"){
                    report_v2pUsingTLB_PTwalk(trace.addr, offsetMask, true, true); 
                }
                if (outputMode == "virtual2physical"){
                    report_virtual2physical(vpnAddr, (currentFrame << offsetSize)^(vpnAddr & offsetMask1)); 
                }
                if (outputMode == "vpn2pfn"){
                    report_pagemap(pageTable->LevelCount, pageList, currentFrame); 
                } 
                count++;
            }
        }
    }	

    if (outputMode == "summary"){
        unsigned int totalBytes = pageTable->EntryCount[0] * 4;
        for (int i=0; i<pageTable->LevelCount-1; i++){
            totalBytes += (pageTable->EntryCount[i+1] * pageTable->bytesCounter[i]*4);
        }
        report_summary(pow(2, offsetSize), cacheHitCount, pageHits, count, frameCount, totalBytes * 2 + sizeof(pageTable));
    }

    /* clean up and return success */
    std::fclose(tracefile);
    return (0);
}
