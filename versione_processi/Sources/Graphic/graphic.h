/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef GRAPHIC
#define GRAPHIC

/* Direttive al Preprocessore */
/* Direttive Locali */
#include "../../Main/utilities.h"

/* Direttive di Sistema */
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

/* Inizializzazione Sottoprogrammi */
void draw_TIME();

void draw_tiles(WINDOW* frame, Tiles tiles[GAME_H][GAME_W]);

void draw_frog(WINDOW* frame, Frog frog, Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], _Bool rainbow, _Bool armor);

void draw_car (WINDOW* frame, Car car);

void draw_car1_sx (WINDOW* frame, Car car);

void draw_car2_sx (WINDOW* frame, Car car);

void draw_car3_sx (WINDOW* frame, Car car);

void draw_car4_sx (WINDOW* frame, Car car);

void draw_car5_sx (WINDOW* frame, Car car);

void draw_car1_dx (WINDOW* frame, Car car);

void draw_car2_dx (WINDOW* frame, Car car);

void draw_car3_dx (WINDOW* frame, Car car);

void draw_car4_dx (WINDOW* frame, Car car);

void draw_car5_dx (WINDOW* frame, Car car);

void draw_log(WINDOW* frame, Log log);

void draw_timer(WINDOW* frame, Timer timer, _Bool timeless);

void init_colors();

void init_cp();

void draw_score(WINDOW* frame, int score);

void draw_hp(WINDOW* frame, int hp, _Bool immortal);

void draw_hitboxes(WINDOW* frame, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], _Bool rainbow);

void draw_bullet(WINDOW* frame, Bullet bullet, Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

void draw_frame_bullet(WINDOW *frame, int num_proiettili);

void draw_power(WINDOW *frame, int power);

#endif
