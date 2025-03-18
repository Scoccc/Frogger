/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* INCLUDE GUARD */
#ifndef BULLET
#define BULLET

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

/* Direttive Locali */
#include "../../Main/utilities.h"

/* Definizione Prototipi */
void* start_bullet(void* init_bullet);

Bullet set_frog_bullet(Frog frog, int charge);

Bullet set_enemy_bullet(Log log, int speed);

Packet pack_bullet(Bullet bullet);

Bullet unpack_bullet(Packet pkt);

#endif
