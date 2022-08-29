// #Names: Brian Huang, Andrew Chung
// #RedIDs: 822761804, 821542300
// #Course: CS 480-01
// #Assignment 4

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <chrono>
#include <thread>

using namespace std;

#include "exec.h"
#include "io.h"

void* produce(void* argument)
{
    RequestType whichProducer;
    EXEC_STATUS *producer = (EXEC_STATUS*)argument;

    //mutex lock to access shared data to determine which thread gets which producer
    sem_wait(&producer->mutexLock);
    if(producer->producerFlag == false)
    {
        producer->producerFlag = true;
        whichProducer = HumanDriver;

    }
    else
    {
        whichProducer = RoboDriver;
    }
    //give mutex lock back
    sem_post(&producer->mutexLock);

    //continue to run until producedCount == maxProduce, increment producedCount by 1 each run
    while(producer->producedCount < producer->maxProduce)
    {
        //if this is the human driver producer thread enter
        if(whichProducer == HumanDriver)
        {
            //decrement semaphores to show this thread is accessing the information
            sem_wait(&producer->humanEmpty);
            sem_wait(&producer->semEmpty);
            sem_wait(&producer->mutexLock);
            //checks at the end of production to make sure if a production thread is asleep while another thread 
            //finishes prodctuction it will properly exit rather than producing another item
            if(producer->producedCount == producer->maxProduce)
            {
                sem_post(&producer->mutexLock);
                sem_post(&producer->humanFull);
                sem_post(&producer->semFull);  
                break;
            }
            //add the item onto the queue
            producer->boundedBuffer.push_back(whichProducer);
            //increment the total amount of whichever item in the queue
            producer->inQueue[whichProducer]++;
            //increment the whole total of whichever item was produced
            producer->totalProduced[whichProducer]++;
            //print the statements
            io_add_type(whichProducer, producer->inQueue, producer->totalProduced);
            //essentially finished the while loop so increment producedCount by 1
            producer->producedCount++;
            //increment semaphores to give lock back and give access information to others
            sem_post(&producer->mutexLock);
            sem_post(&producer->humanFull);
            sem_post(&producer->semFull);
            //sleep to represent consumption
            std::this_thread::sleep_for(std::chrono::milliseconds(producer->sleepProducer[whichProducer]));
        }
        else
        {
            //decrement semaphores to show this thread is accessing the information
            sem_wait(&producer->semEmpty);
            sem_wait(&producer->mutexLock);
            //checks at the end of production to make sure if a production thread is asleep while another thread 
            //finishes prodctuction it will properly exit rather than producing another item
            if(producer->producedCount == producer->maxProduce)
            {
                sem_post(&producer->mutexLock);
                sem_post(&producer->semFull);
                break;
            }
            //add the item onto the queue
            producer->boundedBuffer.push_back(whichProducer);
            //increment the total amount of whichever item in the queue
            producer->inQueue[whichProducer]++;
            //increment the whole total of whichever item was produced
            producer->totalProduced[whichProducer]++;
            //print the statements
            io_add_type(whichProducer, producer->inQueue, producer->totalProduced);
            //essentially finished the while loop so increment producedCount by 1
            producer->producedCount++;
            //increment semaphores to give lock back and give access information to others
            sem_post(&producer->mutexLock);
            sem_post(&producer->semFull);
            //sleep to represent consumption
            std::this_thread::sleep_for(std::chrono::milliseconds(producer->sleepProducer[whichProducer]));
        }
    }
}