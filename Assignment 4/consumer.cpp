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

void* consume(void* argument)
{
    ConsumerType whichConsumer;
    EXEC_STATUS *consumer = (EXEC_STATUS*)argument;

    //mutex lock to access shared data to determine which thread gets which consumer
    sem_wait(&consumer->mutexLock);
    if(consumer->consumerFlag == false)
    {
        consumer->consumerFlag = true;
        whichConsumer = CostAlgoDispatch;
    }
    else
    {
        whichConsumer = FastAlgoDispatch;
    }
    //give mutex lock back
    sem_post(&consumer->mutexLock);

    //continue to run until totalConsumed == maxProduce, increment totalConsumed by 1 each run
    while(consumer->totalConsumed < consumer->maxProduce) //check if the last producer has been consumed
    {
        //decrement semaphores to show this thread is accessing the information
        sem_wait(&consumer->semFull);
        sem_wait(&consumer->mutexLock);
        //checks at the end of consumption to make sure if a consumption thread is asleep while another thread 
        //finishes the last consumption it will properly exit rather than trying to consume another item with nothing there
        if (consumer->totalConsumed == consumer->maxProduce)
        {
            sem_post(&consumer->mutexLock);
            break;
        }
        //if the front of the buffer is a human driver requests decrement the semaphore to signify consumption *not needed supposedly*
        if (consumer->boundedBuffer.front() == HumanDriver)
        {
            sem_wait(&consumer->humanFull);
        }
        //decrement the amount in the queue count because of consumption
        consumer->inQueue[consumer->boundedBuffer.front()]--;
        //if statement to determine which consumer to properly increment counts in array
        if (whichConsumer == CostAlgoDispatch)
        {
            consumer->totalCost[consumer->boundedBuffer.front()]++;
            consumer->totalConsumed++;
            io_remove_type(whichConsumer, consumer->boundedBuffer.front(), consumer->inQueue, consumer->totalCost);
        }
        else
        {
            consumer->totalFast[consumer->boundedBuffer.front()]++;
            consumer->totalConsumed++;
            io_remove_type(whichConsumer, consumer->boundedBuffer.front(), consumer->inQueue, consumer->totalFast);
        }
        //if statement to increment back to the human driver semaphore *not needed supposedly*
        if (consumer->boundedBuffer.front() == HumanDriver)
        {
            //erase the item to show consumption
            consumer->boundedBuffer.erase(consumer->boundedBuffer.begin());
            //increment semaphores to give lock back and give access information to others
            sem_post(&consumer->mutexLock);
            sem_post(&consumer->semEmpty);
            sem_post(&consumer->humanEmpty);
        }
        else
        {
            //erase the item to show consumption
            consumer->boundedBuffer.erase(consumer->boundedBuffer.begin());
            //increment semaphores to give lock back and give access information to others
            sem_post(&consumer->mutexLock);
            sem_post(&consumer->semEmpty);
        }
        //sleep to represent consumption
        std::this_thread::sleep_for(std::chrono::milliseconds(consumer->sleepConsumer[whichConsumer]));
    }
}