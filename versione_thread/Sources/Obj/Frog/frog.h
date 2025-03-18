/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* INCLUDE GUARD */
#ifndef FROG
#define FROG

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

/* Direttive Locali */
#include "../../Main/utilities.h"

/* Definizione Prototipi */
void* start_frog();

Packet pack_input(int input);

#endif
