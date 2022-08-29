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
#include <getopt.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

#include "exec.h"
#include "producer.h"
#include "consumer.h"
#include "io.h"

int main(int argc, char** argv)
{
    int option;
    //initialize shared data structure
    struct EXEC_STATUS *start = new EXEC_STATUS();
    //set defautls if no optional arguements are given
    start->sleepConsumer[CostAlgoDispatch] = DEFAULTCOSTTIME;
    start->sleepConsumer[FastAlgoDispatch] = DEFAULTFASTTIME;
    start->sleepConsumer[HumanDriver] = DEFAULTHUMANTIME;
    start->sleepConsumer[RoboDriver] = DEFAULTROBOTIME;

    //initialize semaphores
    sem_init(&start->mutexLock, 0 , 1); //binary semaphore for binary lock
    sem_init(&start->semEmpty, 0, MAXQUEUE); //semaphore set to tweleve for maximum queue size
    sem_init(&start->humanEmpty, 0, MAXHDR); //semaphore set to four for maximum human requests in queue size
    sem_init(&start->semFull, 0, 0); //semaphore set to 0 to show how many requests can be consumed (max 12)
    sem_init(&start->humanFull, 0, 0); //semaphore set to 0 to show how many human requests can be consumed (max 4)
    
    //grab optional arguements
    while((option = getopt(argc, argv, "n:c:f:h:a:")) != -1)
    {
        switch(option)
        {
            case 'n':
                start->maxProduce = atoi(optarg);
                break;
            case 'c':
                start->sleepConsumer[CostAlgoDispatch] = atoi(optarg);
                break;
            case 'f':
                start->sleepConsumer[FastAlgoDispatch] = atoi(optarg);
                break;
            case 'h':
                start->sleepProducer[HumanDriver] = atoi(optarg);
                break;
            case 'a':
                start->sleepProducer[RoboDriver] = atoi(optarg);
                break;
        }
    }
    //counter for comparing max in production threads
    start->producedCount = 0;

    //initialize pthreads for two producers and consumers
    pthread_t humanProducer;
    pthread_t roboProducer;
    pthread_t costConsumer;
    pthread_t fastConsumer;
    pthread_create(&humanProducer,NULL,&produce,start);
    pthread_create(&roboProducer,NULL,&produce,start);
    pthread_create(&costConsumer,NULL,&consume,start);
    pthread_create(&fastConsumer,NULL,&consume,start);


    pthread_join(humanProducer, NULL);
    pthread_join(roboProducer, NULL);
    pthread_join(costConsumer, NULL);
    pthread_join(fastConsumer, NULL);

    // if(pthread_join(costConsumer, NULL) == 0)
    // {
    //     std::cout << "consumer"  << '\n';
    //     pthread_exit(&fastConsumer);
    // }
    // else if(pthread_join(fastConsumer, NULL) == 0)
    // {
    //     std::cout << "fast"  << '\n';
    //     pthread_exit(&costConsumer);
    // }

    // std::cout <<  "asd" << '\n';

    //print out report
    io_production_report(start->totalProduced, start->totalConsume);

    //destroy semaphores
    sem_destroy(&start->semEmpty);
    sem_destroy(&start->semFull);
    sem_destroy(&start->humanEmpty);
    sem_destroy(&start->humanFull);
    sem_destroy(&start->mutexLock);

    return 0;
}