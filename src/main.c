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
	
	nThreads = 0;
	
	// arg 1: number of threads
	if (argc > 1) {
	    nThreads= atoi(argv[1]);
	}
	
	// limit number of threads
	nThreads = (nThreads > MAX_THREADS) ? MAX_THREADS : nThreads;
	
    // DEBUG
    printf("before:\n");
    for (int i = 0; i < sizeof(dummySortDataMicro) / sizeof(int); i++) {
        printf("%d ", dummySortDataMicro[i]);
    }
    printf("\n");
	
	// setup timers
	initCTimer(cp, PROCESS);
	initCTimer(ce, MONOTONIC);
	startCTimer(cp);
	startCTimer(ce);

	parSort(dummySortDataMicro, sizeof(dummySortDataMicro) / sizeof(int), nThreads, 0);

    stopCTimer(cp);
    stopCTimer(ce);
    double ptime = getCTime(cp);
    double etime = getCTime(ce);
    
    // DEBUG
    printf("after:\n");
    for (int i = 0; i < sizeof(dummySortDataMicro) / sizeof(int); i++) {
        printf("%d ", dummySortDataMicro[i]);
    }
    printf("\n");

    printf("Elapsed: %2.2f, process %2.2f, "\
           "average per CPU %2.2f)\n", 
                    etime, ptime, ptime/((double)nThreads));
}

//******************************************************************************

