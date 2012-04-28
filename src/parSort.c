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

void resetChunks(chunk_t *chunks, int nThreads) {
    int i;
    
    for (i = 0; i < nThreads; i++) {
        chunks[i].idx = 0;
    }
}

sort_t *getLeast(chunk_t *chunks, int nThreads) {
    chunk_t *chunkOne;
    chunk_t *chunkTwo;
    sort_t  *dataOne;
    sort_t  *dataTwo;
    int     start;
    int     i;
    
    chunkOne = NULL;
    
    // .
    for (i = 0; i < nThreads; i++) {
        if (chunks[i].idx < chunks[i].len) {
            chunkOne = &chunks[i];
            dataOne  = &chunkOne->data[chunkOne->idx];
            start    = i + 1;
            break;
        }
    }
    
    //printf("--- first ---\n");
    //printf("chunk i=%d idx=%d data=%d\n", start - 1, chunkOne->idx, *dataOne);
    
    for (i = start; i < nThreads; i++) {
        if (chunks[i].idx < chunks[i].len) {
            chunkTwo = &chunks[i];
            dataTwo  = &chunkTwo->data[chunkTwo->idx];
            
            //printf("chunk i=%d idx=%d data=%d\n", i, chunkTwo->idx, *dataTwo);
            
            //printf("compare: %i < %i\n", *dataOne, *dataTwo);
            if (*dataTwo < *dataOne) {
                //printf("switch\n");
                chunkOne = chunkTwo;
                dataOne  = &chunkOne->data[chunkOne->idx];
            }
        }
    }
    
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
    long        start;
    long        i;
    
    // BUG???
    if(nThreads < 1) {
        nThreads = getNumCpus();
    }
    
    // divide problem into chucks => set chunk size
    chunkSize = len / nThreads;
    start     = 0;
    
    //printf("sizeof(sort_t) = %d bytes\n", sizeof(sort_t));
    
    // 
    for (i = 0; i < nThreads; i++) {
        
        // If this is the last thread
        // chunk-end = array-length
        // minimum of theads = 1
        if(i + 1 == nThreads) {
            chunks[i].len = len - ((nThreads - 1) * chunkSize);
        } else {
            chunks[i].len = chunkSize;
        }
        
        size_t allocBytes = chunks[i].len * sizeof(sort_t);
        //printf("alloc = %d bytes\n", allocBytes);
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

    // DEBUG
    /*
    printf("before:\n");
    x = 0;
    for(i = 0; i < len; i++) {
        printf("%02i, ", data[i]);
    }
    printf("\n");
    
    
    printf("after:\n");
    x = 0;
    for(i = 0; i < nThreads; i++) {
        for (k = 0; k < chunks[i].len; k++) {
            printf("%02i, ", chunks[i].data[k]);
        }
        printf("\n");
    }
    */
    
    // Merge
    // over original array-length
    resetChunks(chunks, nThreads);
    for (i = 0; i < len; i++) {
        //printf("least: %i\n", *getLeast(chunks, nThreads));
        data[i] = *getLeast(chunks, nThreads);
    }
    
    // Deallocate memory

    for (i = 0; i < nThreads; i++) {
        free(chunks[i].data);
    }
}


//******************************************************************************

