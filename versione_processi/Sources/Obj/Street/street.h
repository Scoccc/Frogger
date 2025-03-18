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

void start_car(int fd[2], int CtoG[2]);

int rollD100();

/* Fine Definizioni Libreria */
#endif
