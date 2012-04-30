#ifndef __PAR_SORT_H__
#define __PAR_SORT_H__

//******************************************************************************
// Course:  MPC Project
// Author:  Andreas Butti, Andreas Bachmann
// Purpose: MPC Sort Project
//******************************************************************************

//******************************************************************************
// API

// type defintion für Sortierung
typedef double sort_t;

/**
 * - sort t *data: Array, Lange len
 * - sort t: numerischer Skalartyp definiert in parSort.h
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
void parSort(sort_t *data, int len, int nThreads, int thresh);

//******************************************************************************

#endif

