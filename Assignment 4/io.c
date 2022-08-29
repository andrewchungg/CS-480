#include <stdio.h>
#include <time.h>

#include "io.h"

/*
 * i/o functions - assumed to be called in a critical section
 */


/* Handle C++ namespaces, ignore if compiled in C 
 * C++ usually uses this #define to declare the C++ standard.
 * It will not be defined if a C compiler is used.
 */
#ifdef __cplusplus
using namespace std;
#endif

/*
 * Data section - names must align with the enumerated types
 * defined in ridesharing.h
 */

/* Names of producer threads and request types */
const char *producerNames[] = {"Human Driver Request", "Robot Driver Request"};
const char *producerAbbrevs[] = {"HDR", "RDR"};

/* Names of consumer threads */
const char *consumerNames[] = {"CostAlgo Dispatcher", "FastAlgo Dispatcher"};

/* double elapsed_s()
 * show seconds of wall clock time used by the process
 */

double elapsed_s() {
  const double ns_per_s = 1e9; /* nanoseconds per second */

  /* Initialize the first time we call this */
  static timespec start;
  static int firsttime = 1;

  struct timespec t;

  /* get elapsed wall clock time for this process
   * note:  use CLOCK_PROCESS_CPUTIME_ID for CPU time (not relevant here
   * as we will be sleeping a lot on the semaphores)
   */
  clock_gettime(CLOCK_REALTIME, &t);

  if (firsttime) {
    /* first time we've called the function, store the current
     * time.  This will not track the cost of the first item
     * produced, but is a reasonable approximation for the
     * whole process and avoids having to create an initialization
     * function.
     * (In C++, we'd just build a timer object and pass it around,
     *  but this approximation provides a simple interface for both
     *  C and C++.)
     */
    firsttime = 0;  /* don't do this again */
    start = t;  /* note when we started */
  }
  
  /* determine time delta from start and convert to s */
  double s = (t.tv_sec - start.tv_sec) + 
    (t.tv_nsec - start.tv_nsec) / ns_per_s ;
  return s;
}

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
void io_add_type(RequestType requestType, int inRequestQueue[], int produced[]) {
  int idx;
  int total;

  /* Show what is in the broker request queue */
  printf("Broker: ");
  total = 0;  /* total produced */
  for (idx=0; idx < RequestTypeN; idx++) {
    if (idx > 0)
      printf(" + ");  /* separator */
    printf("%d %s", inRequestQueue[idx], producerAbbrevs[idx]);
    total += inRequestQueue[idx];
  }

  printf(" = %d. ", total);

  printf("Added %s.", producerNames[requestType]);

  /* Show what has been produced */
  total = 0;
  printf(" Produced: ");
  for (idx=0; idx < RequestTypeN; idx++) {
    total += produced[idx];  /* track total produced */
    if (idx > 0)
      printf(" + ");  /* separator */
    printf("%d %s", produced[idx], producerAbbrevs[idx], produced[idx]);
  }
  /* total produced over how long */
  printf(" = %d in %.3f s.\n", total, elapsed_s());
  //printf(" = %d\n", total);

  /* This is not really needed, but will be helpful for making sure that you
   * see output prior to a segmentation vioilation.  This is not usually a
   * good practice as we want to avoid ending the CPU burst premaurely which
   * this will do, but it is a helpful technique.
   */
  fflush(stdout);  
};

/*
 * io_remove_type(Consumers consumer, RequestType requestType, 
 *     int inRequestQueue[], int consumed[])
 * Show that an item / request has been removed from the request queue 
 * and print the current status of the broker request queue.
 * 
 * Expects:
 * consumer - Who removed / dispatched the request?
 * requestType:  What kind of request was removed / dispatched?
 * inRequestQueue:  Array of number of requests of each type that are
 *   in the request queue and have not yet been consumed / dispatched.
 *   (inRequestQueue[HumanDriver] and inRequestQueue[RoboDriver])
 * consumed:  Array of number of requests of each type that have been
 *  consumed / dispatched by consumer / dispatcher.
 *
 * Counts reflect numbers after the request has been removed / dispatched
 */
void io_remove_type(ConsumerType consumer, RequestType requestType,
		    int inRequestQueue[], int consumed[]) {
  int idx;
  int total;

  /* Show what is in the broker request queue */
  total = 0;
  printf("Broker: ");
  for (idx=0; idx < RequestTypeN; idx++) {
    if (idx > 0)
      printf(" + ");  /* separator */
    printf("%d %s", inRequestQueue[idx], producerAbbrevs[idx]);
    total += inRequestQueue[idx];
  }
  printf(" = %d. ", total);

  
  /* Show what has been consumed by consumer */
  printf("%s consumed %s.  %s totals: ",
    consumerNames[consumer],
    producerNames[requestType],
    consumerNames[consumer]);
  total = 0;
  for (idx = 0; idx < RequestTypeN; idx++) {
    if (idx > 0)
      printf(" + ");  /* separator */
    total += consumed[idx];  /* track total consumed */
    printf("%d %s", consumed[idx], producerAbbrevs[idx]);
  }
  /* total consumed over how long */
  printf(" = %d consumed in %.3f s.\n", total, elapsed_s());
  //printf(" = %d consumed\n", total);

  /* This is not really needed, but will be helpful for making sure that you
   * see output prior to a segmentation vioilation.  This is not usually a
   * good practice as we want to avoid ending the CPU burst premaurely which
   * this will do, but it is a helpful technique.
   */
  fflush(stdout);
};


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
void io_production_report(int produced[], int *consumed[]) {
  int p, c;  /* array indices */
  int total;

  printf("\nREQUEST REPORT\n----------------------------------------\n");
   
  /* show number produced for each producer / request type */
  for (p = 0; p < RequestTypeN; p++) {
    printf("%s producer generated %d requests\n",
	   producerNames[p], produced[p]);
  }
  /* show number consumed by each consumer */
  for (c=0; c < ConsumerTypeN; c++) {
    printf("%s consumed ", consumerNames[c]);
    total = 0;
    for (p = 0; p < RequestTypeN; p++) {
      if (p > 0)
	printf(" + ");
    
      total += consumed[c][p];
      printf("%d %s", consumed[c][p], producerAbbrevs[p]);
    }
    printf(" = %d total\n", total);
  }

  printf("Elapsed time %.3f s\n", elapsed_s());
}
