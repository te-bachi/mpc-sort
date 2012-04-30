//******************************************************************************
// Course:  MPC Project
// Author:  Andreas Butti
// Purpose: MPC Sort Project
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mtimer.h"
#include "parSort.h"
#include "dummyData.h"

//******************************************************************************
// constant values

#define MAX_THREADS  16

//******************************************************************************
// main program

int main(int argc, char *argv[]) {
	ctimer_t cp;
	ctimer_t ce;
	int nThreads;
	sort_t *sort    = dummyDoubleSortData;
	size_t  sortLen = sizeof(dummyDoubleSortData) / sizeof(sort_t);
	
	nThreads = 0;
	
	// arg 1: number of threads
	if (argc > 1) {
	    nThreads= atoi(argv[1]);
	}
	
	// limit number of threads
	nThreads = (nThreads > MAX_THREADS) ? MAX_THREADS : nThreads;
	
    // DEBUG
    /*
    printf("before:\n");
    for (int i = 0; i < sortLen; i++) {
        printf("%d ", sort[i]);
    }
    printf("\n");
	*/
	
	// setup timers
	initCTimer(cp, PROCESS);
	initCTimer(ce, MONOTONIC);
	startCTimer(cp);
	startCTimer(ce);

	parSort(sort, sortLen, nThreads, 0);

    stopCTimer(cp);
    stopCTimer(ce);
    double ptime = getCTime(cp);
    double etime = getCTime(ce);
    
    // DEBUG
    printf("after:\n");
    for (int i = 0; i < sortLen; i++) {
        printf("%12.4f ", sort[i]);
        if (i != 0 && i % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    
    printf("Elapsed: %2.2f, process %2.2f, "\
           "average per CPU %2.2f)\n", 
                    etime, ptime, ptime/((double)nThreads));
}

//******************************************************************************

