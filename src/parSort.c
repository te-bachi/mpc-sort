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

//******************************************************************************
// constant values

#define MAX_THREADS  16

//******************************************************************************
// internal structure

typedef struct {
	sort_t *data;
	int start;
	int end;
} sort_internal_t;


//******************************************************************************
// quicksort implementation

int compare(const void *a, const void *b) {
	return ( *(sort_t*)a - *(sort_t*)b );
}

//******************************************************************************
// thread function


void *sortThread(void *arg) {
    sort_internal_t *data = (sort_internal_t *)(arg);

	qsort(&data->data[data->start], data->end - data->start, sizeof(sort_t), compare);

	return NULL;
}

//******************************************************************************
// API

/**
 * - sort_t *data: Array, Lange len
 * - sort_t: numerischer Skalartyp definiert in parSort.h
 * 
 * - int len: Lange des Arrays, 100’000 ≤ len ≤ 1000’000.
 * - int nThreads: maximale Anzahl Threads
 * 
 * nThreads ≤ 0: Sortierverfahren wahlt Anzahl Threads selbst
 * 
 * - int thresh: Arraylange, ab welcher sequentiell sortiert werden soll
 *               thresh ≤ 0: Sortierverfahren wahlt thresh selbst
 * 
 * - mindestens einer der beiden Parameter nThreads resp. thresh muss 0 sein
 * 
 * - aufsteigend sortiert, Datenruckgabe in-place sortiert in data
 * - Starten und Stoppen der Threads in der Sortierfunktion parSort()
 */
void parSort(sort_t *data, int len, int nThreads, int thresh) {
	// TODO berechnen der Threads!!
	if(nThreads < 1) {
		nThreads = 4;
	}

    pthread_t th[nThreads];
	sort_internal_t id[nThreads];
	int partCount = len / nThreads;

	int start = 0;
	// create threads and pass thread number
	for (long i = 0; i < nThreads; i++) {
		id[i].start = start;

		if(i + 1 == nThreads) {
			id[i].end = len;
		} else {
			id[i].end = start + partCount;
		}
		id[i].data = data;
		start += partCount;

		printf("%li: start=%i / end=%i\n", i, id[i].start, id[i].end);

		pthread_create(&th[i], NULL, sortThread, (void *)&id[i]);
	}

	// wait for threads to terminate    
	for (int i = 0; i < nThreads; i++) {
		pthread_join(th[i], NULL);
	}

	int x = 0;
	for(int i = 0; i < len; i++) {
		printf("%02i, ", data[i]);

		if(id[x].end == i + 1) {
			x++;
			printf("\n\n");
		}
	}

	// TODO mergen
}


//******************************************************************************

