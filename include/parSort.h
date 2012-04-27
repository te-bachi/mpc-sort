//******************************************************************************
// Course:  MPC Project
// Author:  Andreas Butti
// Purpose: MPC Sort Project
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>

//******************************************************************************
// API

// type defintion für Sortierung
typedef int sort_t;


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

