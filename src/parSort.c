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
    sort_t  *data;
    int     idx;
    int     len;
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
    
    qsort(chunk->data, chunk->len, sizeof(sort_t), compare);
    
    return NULL;
}

//******************************************************************************
// reset chunk indexes

void resetIndexes(chunk_t *chunks, int nThreads) {
    int i;
    
    for (i = 0; i < nThreads; i++) {
        chunks[i].idx = 0;
    }
}

//******************************************************************************
// get least data entry of a chunk-list

/**
 * ex. nThreads = 4
 *
 * chunks[i].idx = 0
 * chunks[i].len = 10
 * 
 *            +---+------
 * chunks[0]  | 5 | [...]
 *            +---+------
 * chunks[1]  | 3 | [...]
 *            +---+------
 * chunks[2]  | 1 | [...]
 *            +---+------
 * chunks[3]  | 4 | [...]
 *            +---+------
 *
 */
sort_t *getLeast(chunk_t *chunks, int nThreads) {
    chunk_t *chunkOne;
    chunk_t *chunkTwo;
    sort_t  *dataOne;
    sort_t  *dataTwo;
    int     start;
    int     i;
    
    chunkOne = NULL;
    
    // take first data entry
    for (i = 0; i < nThreads; i++) {
        // take only, if index is not out of range
        if (chunks[i].idx < chunks[i].len) {
            chunkOne = &chunks[i];
            dataOne  = &chunkOne->data[chunkOne->idx];
            
            // start condition for next loop
            start    = i + 1;
            break;
        }
    }
    
    // loop only over the rest of the chunk-list
    for (i = start; i < nThreads; i++) {
        if (chunks[i].idx < chunks[i].len) {
            chunkTwo = &chunks[i];
            dataTwo  = &chunkTwo->data[chunkTwo->idx];
            
            // is it less than?
            if (*dataTwo < *dataOne) {
                // overwrite
                chunkOne = chunkTwo;
                dataOne  = &chunkOne->data[chunkOne->idx];
            }
        }
    }
    
    // increment index of least data entry
    if (chunkOne != NULL) {
        chunkOne->idx++;
    }
    
    return dataOne;
}

//******************************************************************************
// API

void parSort(sort_t *data, int len, int nThreads, int thresh) {
    pthread_t   th[nThreads];
    chunk_t     chunks[nThreads];
    
    int         chunkSize;
    size_t      allocBytes;
    long        start;
    long        i;
    
    // BUG???
    if(nThreads < 1) {
        nThreads = getNumCpus();
    }
    
    // divide problem into chucks => set chunk size
    chunkSize = len / nThreads;
    start     = 0;
    
    // 
    for (i = 0; i < nThreads; i++) {
        
        // if this is the last thread => take the rest
        if(i + 1 == nThreads) {
            chunks[i].len = len - ((nThreads - 1) * chunkSize);
        } else {
            chunks[i].len = chunkSize;
        }
        
        // allocate chunk data and copy it
        allocBytes = chunks[i].len * sizeof(sort_t);
        chunks[i].data = (sort_t *) malloc(allocBytes);
        memcpy(chunks[i].data, &data[start], allocBytes);

        // DEBUG
        printf("%li: len=%i\n", i, chunks[i].len);

        // create threads and pass one of the chunks
        pthread_create(&th[i], NULL, sortThread, (void *)&chunks[i]);
        
        start += chunkSize;
    }

    // wait for threads to terminate    
    for (i = 0; i < nThreads; i++) {
        pthread_join(th[i], NULL);
    }
    
    // merge chunk list into original data-array
    resetIndexes(chunks, nThreads);
    for (i = 0; i < len; i++) {
        data[i] = *getLeast(chunks, nThreads);
    }
    
    // free chunk data
    for (i = 0; i < nThreads; i++) {
        free(chunks[i].data);
    }
}


//******************************************************************************

