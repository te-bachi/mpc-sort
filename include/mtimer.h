#ifndef TIMER_MACRO_DEFINITIONS
#define TIMER_MACRO_DEFINITIONS

/******************************************************************************/
// File:    mtTimer.h
// Purpose: timer functions (macros) based on clock_gettime() and times()
// Linking: needs rt library (-lrt)
// Author:  M. Thaler, 11/2011
//
/******************************************************************************/
// clock_gettime()
//  function:   elapsed time:   ctimerType = REALTIME or MONOTONIC
//              per process:    ctimerType = PROCESS
//              per thread:     ctimerType = THREAD
//  date type:  ctimer_t
//  functions:  initCTimer(ctimer_t tdata, gtimerType tp)   -> setup timer
//              startCTimer(ctimer_t tdata)                 -> start timer
//              stopCTimer(ctimer_t tdata)                  -> stop  timer
//              double getCTime(ctimer_t tdata)             -> get time in s
//              printCTime(X)                               -> print time in s
//
//  -> see also "man clock_gettime"  ... and read the NOTES entry
//------------------------------------------------------------------------------
// gettimeofday()
//  function:   elapsed time:   between start and stop
//  data type:  gtimer_t
//  functions:  startGTimer(gtimer_t tdata)                 -> start timer
//              stopGTimer(gtimer_t tdata)                  -> stop  timer
//              double getWallGTime(gtimer_t tdata)         -> get time in s
//              printGTime(X)                               -> print time in s
// 
//  -> see also "man gettimeofday"
//------------------------------------------------------------------------------
// times()
//  function:   per process:   system time and user user time
//              per child:     system time and user time when terminated
//                             (and waited for) 
//              elapsed time:  between start and end  
//  date type:  ttimer_t
//  functions:  startTTimer(gtimer_t tdata) -> start timer
//              stopTTimer(gtimer_t tdata)  -> stop  timer
//              double getUsrTime(X)        -> get total user time ins
//              double getSysTime(X)        -> get total system time in s
//              double getCUsrTime(X)       -> get user time of children in s
//              double getCUSysTime(X)      -> get system time of children in s
//              double getWallTime(X)       -> get elapsed time in s
//              printTTime(X)               -> print time in s
//
//  -> see also "man times"
/******************************************************************************/

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>


/******************************************************************************/
enum ctimerType { REALTIME  = CLOCK_REALTIME,
                  MONOTONIC = CLOCK_MONOTONIC,
                  PROCESS   = CLOCK_PROCESS_CPUTIME_ID, 
                  THREAD    = CLOCK_THREAD_CPUTIME_ID };

typedef struct ctimer_t { struct timespec sT; \
                          struct timespec eT; \
                          clockid_t       clockID; } ctimer_t;
  
#define initCTimer(X,Y)  X.clockID = Y
#define startCTimer(X)   clock_gettime(X.clockID, &(X.sT))
#define stopCTimer(X)    clock_gettime(X.clockID, &(X.eT))
#define getCTime(X)      ((double)(X.eT.tv_sec) - (double)(X.sT.tv_sec)) +\
                            ((double)X.eT.tv_nsec  - (double)X.sT.tv_nsec)/1e9

#define printCTime(X)       switch (X.clockID) { \
                                case REALTIME :  \
                                    printf("elapsed %3.4lfs\n", getCTime(X)); \
                                    break;       \
                                case MONOTONIC:  \
                                    printf("elapsed %3.4lfs\n", getCTime(X)); \
                                    break;       \
                                case PROCESS:    \
                                    printf("process %3.4lfs\n", getCTime(X)); \
                                    break;       \
                                case THREAD:     \
                                    printf("thread %3.4lfs\n", getCTime(X));  \
                                    break;       \
                                default:         \
                                    break;       \
                            }
                                
//------------------------------------------------------------------------------

typedef struct gtimer_t { struct timeval sT; \
                          struct timeval eT; } gtimer_t;

#define startGTimer(X)  gettimeofday(&X.sT, NULL)  
#define stopGTimer(X)   gettimeofday(&X.eT, NULL)
#define getWallGTime(X) ((double)(X.eT.tv_sec) - (double)(X.sT.tv_sec)) +\
                            ((double)X.eT.tv_usec - (double)X.sT.tv_usec)/1e6  
#define printGTime(X)   printf("elapsed %3.4lfs\n", getWallGTime(X))

//------------------------------------------------------------------------------

typedef struct ttimer_t { struct tms sT; \
                          struct tms eT; \
                          clock_t start, end;} ttimer_t;


#define startTTimer(X)  X.start = times(&X.sT)
#define stopTTimer(X)   X.end   = times(&X.eT)

#define getUsrTime(X)   ((double)X.eT.tms_utime - (double)X.sT.tms_utime) / \
                            ((double)sysconf(_SC_CLK_TCK))

#define getSysTime(X)   ((double)X.eT.tms_stime - (double)X.sT.tms_stime) / \
                            ((double)sysconf(_SC_CLK_TCK))

#define getCUsrTime(X)  ((double)X.eT.tms_cutime - (double)X.sT.tms_cutime) / \
                            ((double)sysconf(_SC_CLK_TCK))

#define getCUSysTime(X) ((double)X.eT.tms_cstime - (double)X.sT.tms_cstime) / \
                            ((double)sysconf(_SC_CLK_TCK))

#define getWallTTime(X) ((double)(X.end) - (double)(X.start)) / \
                             ((double)(sysconf(_SC_CLK_TCK)))

#define printTTime(X)   \
    printf("elapsed %3.2lfs, sys %3.2lfs, user %3.2lfs\n", \
           getWallTTime(X), getSysTime(X), getUsrTime(X))

/******************************************************************************/

#endif
                          
