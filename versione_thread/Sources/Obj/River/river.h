/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef RIVER
#define RIVER

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <stdio.h>

/* Direttive Locali */
#include "../../Main/utilities.h"

/* Definizione Prototipi */
void* start_log(void *thread_argument);

Packet pack_log(Log log);

Log unpack_log(Packet pkt);

/* Fine Definizioni Libreria */
#endif
