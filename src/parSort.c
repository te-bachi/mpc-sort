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
	int start;
	int end;
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

sort_t *getLeast(chunk_t *chunk, int nThreads) {
    int    i;
    sort_t *one;
    sort_t *other;
    
    one = &chunk[0].data[chunk[0].start];
    printf("first: %i\n", *one);
    for (i = 1; i < nThreads; i++) {
        other = &chunk[i].data[chunk[i].start];
        printf("compare: %i < %i\n", *one, *other);
        if (*other < *one) {
            printf("switch\n");
            one = other;
        }
    }
    
    // TODO: start raufzählen
    
    return one;
}

//******************************************************************************
// API

void parSort(sort_t *data, int len, int nThreads, int thresh) {
    pthread_t th[nThreads];
	chunk_t   chunk[nThreads];
	sort_t    chunkData[sizeof(sort_t) * len];
	sort_t    *ptr;
	
	int  chunkSize;
	int  start;
	long i, k;
	int  x;
	
	// BUG???
	if(nThreads < 1) {
		nThreads = getNumCpus();
	}
	
	// copy data
	memcpy(chunkData, data, sizeof(sort_t) * len);
	
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
		chunk[i].data = chunkData;
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

	// DEBUG,
	printf("before:\n");
	x = 0;
	for(i = 0; i < len; i++) {
		printf("%02i, ", data[i]);

		if(chunk[x].end == i + 1) {
			x++;
			printf("\n\n");
		}
	}
	
	printf("after:\n");
	x = 0;
	for(i = 0; i < len; i++) {
		printf("%02i, ", chunkData[i]);

		if(chunk[x].end == i + 1) {
			x++;
			printf("\n\n");
		}
	}
	
	// Merge
	// over original array-length
	printf("least: %i\n", *getLeast(chunk, nThreads));
	//for (i = 0; i < len; i++) {
	    
	//}
}


//******************************************************************************

