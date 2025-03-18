/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef STREET
#define STREET

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

/* Direttive Locali */
#include "../../Main/utilities.h"

/* Definizione Prototipi */
Packet pack_car(Car car);

void* start_car();

int rollD100();

Car unpack_car(Packet pkt);

/* Fine Definizioni Libreria */
#endif
