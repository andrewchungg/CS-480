#include "ridesharing.h"

/*
 * io_add_type(RequestType requestType, int inRequestQueue[], int produced[])
 * Show that a request has been added to the request queue and print the current
 * status of the broker request queue.
 * Expects:
 * 
 * requestType:  What kind of request was produced?
 * inRequestQueue:  Array of number of requests of each type that are
 *   in the request queue and have not yet been consumed.
 *   (inRequestQueue[HumanDriver] and inRequestQueue[RoboDriver])
 * produced:  Array of number of requests of each type that have been
 *   produced
 *
 * inRequestQueue and produced reflect numbers *after* adding the current request. 
 */
void io_add_type(RequestType, int inRequestQueue[], int produced[]);

/*
 * io_remove_type(Consumers consumer, RequestType requestType, 
 *     int inRequestQueue[], int consumed[])
 * Show that an item has been removed from the request queue and print the current
 * status of the broker request queue.
 * 
 * Expects:
 * consumer - Who removed (dispatched) the request?
 * requestType:  What kind of request was removed?  As each consumer consumes
 *   one type of request, this is the same as the producer.
 * inRequestQueue:  Array of number of requests of each type that are
 *   in the request queue and have not yet been consumed.
 *   (inRequestQueue[HumanDriver] and inRequestQueue[RoboDriver])
 * Consumed:  Array of number of requests of each type that have been
 *  consumed.
 *
 * Counts reflect numbers *after* the request has been removed
 */
void io_remove_type(Consumers, RequestType, int inRequestQueue[], int consumed[]);

/*
 * void io_production_report(int produced[], int *consumed[])
 * Show how many requests of each type produced.  Show how many requests consumed by
 * each consumer.
 * produced[] - count for each RequestType
 * consumed[] - array of pointers to consumed arrays for each consumer
 *    e.g. consumed[CostAlgoDispatch] points to an array that is indexed by request type
 *    (consumed[CostAlgoDispatch][HumanDriver] is the number for requesting human drivers 
 *    that the cost based algorithm dispatched)
 */
void io_production_report(int produced[], int *consumed[]);
