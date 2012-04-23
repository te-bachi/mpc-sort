#ifndef MY_CPU_INFO_REQUEST
#define MY_CPU_INFO_REQUEST

//******************************************************************************
// Course:    MPC
// Authhor:   M. Thaler, ZHAW
// File:      cpuinfo.h
// Purpose:   get number of CPUs, Cores, Threads
// Functions: int getNumCores(void)   -> umber of cores
//            int getNumThreads(void) -> umber of threads
//            int getNumCpus(void)    -> umber of cpus = cores * threads
//******************************************************************************
//
//
//
//******************************************************************************
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define CPU_INFO "/tmp/MY_CPU_INFO_DATA"
#define THE_CPU_INFO_STRING "lscpu | \
        awk '/^CPU\\(/ {print $2}; \
             /Core/   {print $4}; \
             /Thread/ {print $4};' >  "CPU_INFO""

int readInfo(int which) {
    int data, i;
    system(THE_CPU_INFO_STRING);
    FILE *theFile = fopen(CPU_INFO, "r");
    if ((which < 1) || (which > 3))
        return -1;
    for (i = 0; i < which; i++)
        fscanf(theFile, "%d", &data);
    fclose(theFile);
    unlink(CPU_INFO);
    return data;
}

int getNumCpus(void) {
    return readInfo(1);
}

int getNumThreads(void) {
    return readInfo(2);
}

int getNumCores(void) {
    return readInfo(3);
}
#endif
    
