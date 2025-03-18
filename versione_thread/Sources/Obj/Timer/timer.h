/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef TIMERLIB
#define TIMERLIB

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

/* Direttive Locali */
#include "../../Main/utilities.h"

/* Inizializzazione Sottoprogrammi */
void* start_timer(void* unlimited_time);
Packet pack_timer(Timer t);

/* Fine Definizioni Libreria */
#endif
