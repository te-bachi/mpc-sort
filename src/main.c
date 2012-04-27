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
	ctimer_t cp, ce;

	// enforce one argument
	if (argc < 2) {
		printf("give number of Threads\n");
		exit(0);
	}

	// arg 1: number of threads
	int nThreads = atoi(argv[1]);
	nThreads = (nThreads > MAX_THREADS) ? MAX_THREADS : nThreads;


	// setup timers
	initCTimer(cp, PROCESS);
	initCTimer(ce, MONOTONIC);
	startCTimer(cp);
	startCTimer(ce);

	parSort(dummySortDataMini, sizeof(dummySortDataMini) / sizeof(int), nThreads, 0);

    stopCTimer(cp);
    stopCTimer(ce);
    double ptime = getCTime(cp);
    double etime = getCTime(ce);

    printf("Elapsed: %2.2f, process %2.2f, "\
           "average per CPU %2.2f)\n", 
                    etime, ptime, ptime/((double)nThreads));
}

//******************************************************************************

