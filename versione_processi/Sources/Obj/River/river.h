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
void start_log(int fd_out[2], int fd_in[2], Corrente corrente, Difficolta diff);

Packet pack_log(Log log);

/* Fine Definizioni Libreria */
#endif
