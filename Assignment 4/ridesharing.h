#ifndef RIDESHARING_H
#define RIDESHARING_H

/*
 * Arrays with producer and consumer names
 * These can be indexed with the enumerated types below
 * and are defined in io.c
 */
extern const char *producerNames[];
extern const char *producerAbbrevs[];  
extern const char *consumerNames[]; 

/*
 * Enumerated types to be used by the producers and consumers
 * These are expected in the input/output functions (io.h)
 * should be useful in your producer and consumer code.
 */

/**
 * The broker can hold up to a maximum of 12 undispatched requests 
 * in its request queue at any given time.
 * To promote the use of autonomous cars (as human drivers cost more), 
 * the broker would hold no more than 4 undispatched requests for a human driver at any given time. 
*/

/*
 * Two rider request services (producers of requests) are offered: 
 *   one for publishing (producing) a ride request for a human driver, 
 *   one for publishing (producing) a ride request for an autonomous car.
 * Each ride request service (producer) only publishes (produces) 
 * one type of requests,
 * so RequestType is synomonous with the producer type
 */
typedef enum Requests {
  HumanDriver = 0,   // ride with a human driver
  RoboDriver = 1,  // ride with an autonomous car
  RequestTypeN = 2,   // number of ride request types
} RequestType;

/* 
 * Two dispatcher services (consumers of requests) are available using 
 * different driver search algorithms
 *   one uses a cost saving algorithm to find a nearby driver
 *   one uses a fast-matching algorithm to find a nearby driver 
 *   Requests are taken off from the broker request queue (by dispatchers) 
 *   in the order that they are published (produced).
*/
typedef enum Consumers {
  CostAlgoDispatch = 0,   // dispatch to a nearby driver based on cost
  FastAlgoDispatch = 1,  // dispatch to a nearby driver based on fast matching 
  ConsumerTypeN = 2,   // Number of consumers
} ConsumerType;


#endif
