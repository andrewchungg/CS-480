// #Names: Brian Huang, Andrew Chung
// #RedIDs: 822761804, 821542300
// #Course: CS 480-01
// #Assignment 4

#include <vector>
#include <semaphore.h>
#include <pthread.h>
#include "io.h"

/**
* Shared constants and data structures among threads
**/
#define MAXQUEUE 12
#define MAXHDR 4
#define MINIMUMQUEUE 0
#define DEFAULTHUMANTIME 0
#define DEFAULTROBOTIME 0
#define DEFAULTCOSTTIME 0
#define DEFAULTFASTTIME 0

#ifndef EXECSTATUS
#define EXECSTATUS
struct EXEC_STATUS
{
    //vector to represent the bounded buffer
    std::vector<RequestType> boundedBuffer;
    //two flags to figure out which thread gets which producer/consumer
    bool producerFlag = false;
    bool consumerFlag = false;
    //arrays to hold how long to sleep each producer/consumer
    int sleepProducer[RequestTypeN];
    int sleepConsumer[ConsumerTypeN];
    //counter to compare to max produce
    int producedCount;
    //counter to compare to max produce on consumer side
    int totalConsumed = 0;
    //the maximum amount producers are suppose to produce
    int maxProduce;
    //array to keep track of how many producers in the queue
    int inQueue[RequestTypeN];
    //array to keep track of total amount of produced and total amount for each consumer of how many they consumed
    int totalProduced[RequestTypeN];
    int totalCost[ConsumerTypeN];
    int totalFast[ConsumerTypeN];
    //array to point to the two consumer arrays to print out
    int *totalConsume[ConsumerTypeN] = {totalCost, totalFast};
    // pthread_mutex_t mutexBuffer;

    //semaphores
    sem_t mutexLock;
    sem_t semEmpty;
    sem_t humanEmpty;
    sem_t semFull;
    sem_t humanFull;
};
#endif