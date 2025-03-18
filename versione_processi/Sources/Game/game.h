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

/* Definizione Prototipi */
/* 
 * Function: new_game
 * Permette di avviare una nuova partita
 *
 * Parameters: NULL
 * Returns: NULL
 */
int new_game(); 

/* 
 * Function: start_gameloop
 * Inizializza il ciclo di gioco
 * Cuore principale del gioco
 *
 * Parameters: int fd[2]
 * Returns: NULL
 */
Exit_status start_gameloop(int fd[NUM_PIPES][2], WINDOW** frames, Tiles tiles[GAME_H][GAME_W], int vite, int* score, Difficolta diff, pid_t frog_bullets[NUM_FROG_BULLETS], pid_t enemy_bullets[NUM_ENEMY_BULLETS], Cheat_Code codici[NUM_CHEATS], pid_t pid_logs[NUM_LOGS]);

/* 
 * Function: unpack_frog
 * Spacchetta i pacchetti rana che arrivano tramite pipe
 *
 * Parameters: Packet pck
 * Returns: NULL
 */
int unpack_input(Packet pck);

/* 
 * Function: unpack_car
 * Spacchetta i pacchetti macchina che arrivano tramite pipe
 *
 * Parameters: Packet pkt
 * Returns: NULL
 */
Car unpack_car(Packet pkt);

/* 
 * Function: unpack_timer
 * Spacchetta i pacchetti timer che arrivano tramite pipe
 *
 * Parameters: Packet pck 
 * Returns: NULL
 */
Timer unpack_timer(Packet pkt);

_Bool are_corsie_full(_Bool corsia_libera[NUM_CORSIE]);

Log unpack_log(Packet pkt);

Bullet unpack_bullet(Packet pkt);

Exit_status frog_collision(Frog frog, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], Cheat_Code codici[NUM_CHEATS]);

_Bool car_collision(Car car, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

_Bool log_collision(Log log, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

Bullet_Collision_Status bullet_collision(Bullet bullet, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], _Bool sparato[NUM_LOGS]);

void log_router(int fd[NUM_PIPES][2], int spawn, Log log);

Difficolta scegli_difficolta();

/* Restituisce l'indice del primo slot libero del caricatore.
   Funziona sia per il caricatore dei nemici, che per il caricatore della rana.
   In caso non ci sia nessuno slot libero, la funzione restituisce -1. */
int getFirstFreeSlot(pid_t caricatore[], int num_proiettili);

int researchBullet(pid_t caricatore[], int num_proiettili, pid_t wanted);

/* Fine Definizione Libreria */
#endif
