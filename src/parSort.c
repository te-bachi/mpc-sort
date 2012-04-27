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
#include "cpuinfo.h"

//******************************************************************************
// internal structure

typedef struct {
	sort_t *data;
	size_t start;
	size_t end;
} chunk_t;


//******************************************************************************
// quicksort implementation

int compare(const void *a, const void *b) {
	return ( *(sort_t*)a - *(sort_t*)b );
}

//******************************************************************************
// thread function


void *sortThread(void *arg) {
    chunk_t *chunk = (chunk_t *)(arg);

	qsort(&chunk->data[chunk->start], chunk->end - chunk->start, sizeof(sort_t), compare);

	return NULL;
}

//******************************************************************************
// API

void parSort(sort_t *data, int len, int nThreads, int thresh) {
    pthread_t th[nThreads];
	chunk_t   chunk[nThreads];
	//sort_t    copy[sizeof(sort_t) * len];
	
	int  chunkSize;
	int  start;
	long i;
	//int  x;
	
	if(nThreads < 1) {
		nThreads = getNumCpus();
	}
	
	// allocate and copy data
	//copy = (sort_t *) malloc(sizeof(sort_t) * len); 
	//memcpy(copy, data, sizeof(sort_t) * len);
	
	// divide problem into chucks => set chunk size
	chunkSize = len / nThreads;
	start     = 0;
	
	// 
	for (i = 0; i < nThreads; i++) {
		chunk[i].start = start;

		// If this is the last thread
		// chunk-end = array-length
		if(i + 1 == nThreads) {
			chunk[i].end = len;
		} else {
			chunk[i].end = start + chunkSize;
		}
		chunk[i].data = data;
		start += chunkSize;

		// DEBUG
		printf("%li: start=%i / end=%i\n", i, chunk[i].start, chunk[i].end);

		// create threads and pass one of the chunks
		pthread_create(&th[i], NULL, sortThread, (void *)&chunk[i]);
	}

	// wait for threads to terminate    
	for (i = 0; i < nThreads; i++) {
		pthread_join(th[i], NULL);
	}

	// DEBUG
	/*
	x = 0;
	for(i = 0; i < len; i++) {
		printf("%02i, ", data[i]);

		if(chunk[x].end == i + 1) {
			x++;
			printf("\n\n");
		}
	}
	
	x = 0;
	for(i = 0; i < len; i++) {
		printf("%02i, ", copy[i]);

		if(chunk[x].end == i + 1) {
			x++;
			printf("\n\n");
		}
	}
	*/
	// Merge
	
}


//******************************************************************************

