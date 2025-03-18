/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef GAME
#define GAME

/* Direttive al Preprocessore */
/* Direttive Locali*/
#include "../Graphic/graphic.h"
#include "../Menus/menus.h"
#include "../Obj/obj.h"

/* Direttive di Sistema */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

/* Definizione Prototipi */
int new_game(); 

Exit_status start_gameloop(WINDOW** frames, Tiles tiles[GAME_H][GAME_W], int vite, int* score, Difficolta diff, pthread_t frog_bullets[NUM_FROG_BULLETS], pthread_t enemy_bullets[NUM_ENEMY_BULLETS], Cheat_Code codici[NUM_CHEATS]);

int unpack_input(Packet pck);

Timer unpack_timer(Packet pkt);

_Bool are_corsie_full(_Bool corsia_libera[NUM_CORSIE]);

Exit_status frog_collision(Frog frog, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], Cheat_Code codici[NUM_CHEATS]);

_Bool car_collision(Car car, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

_Bool log_collision(Log log, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

Bullet_Collision_Status bullet_collision(Bullet bullet, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], _Bool sparato[NUM_LOGS]);

void log_router(Log log);

Difficolta scegli_difficolta();

/* Restituisce l'indice del primo slot libero del caricatore.
   Funziona sia per il caricatore dei nemici, che per il caricatore della rana.
   In caso non ci sia nessuno slot libero, la funzione restituisce -1. */
int getFirstFreeSlot(pthread_t caricatore[], int num_proiettili);

int researchBullet(pthread_t caricatore[], int num_proiettili, pthread_t wanted);

/* Fine Definizione Libreria */
#endif
