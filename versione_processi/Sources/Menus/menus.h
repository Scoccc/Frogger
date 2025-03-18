/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Inizio Definizioni Libreria */
#ifndef MENUS
#define MENUS

/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

/* Direttive Locali */
#include "../Main/utilities.h"

/* Inizializzazione Sottoprogrammi */
/* 
 * Function: correct_screen_size
 * Permette di aggiustare le dimensioni del terminale e di
 * fissarle tali fino alla fine dell'esecuzione del programma
 *
 * Parameters: NULL
 * Returns: NULL
 */
void sizes_menu();

/* 
 * Function: game_title
 * Permette di visualizzare il titolo del gioco e i crediti
 *
 * Parameters: NULL
 * Returns: NULL
 */
void title_menu(); 

/* 
 * Function: game_menu
 * Permette di visualizzare il menù di gioco e di scegliere
 * quale opzione eseguire
 *
 * Parameters: NULL
 * Returns: opzione selezionata
 */
Menu game_menu(); 

/* 
 * Function: secret_menu
 * Permette di accedere al menu' segreto, dove si possono inserire codici
 * per sbloccare funzioni segrete
 *
 * Parameters: 
 * Returns: NULL
 */
void secret_menu();

/*
 *
 */
void color_change_menu();

void view_scores_menu();

void game_over(WINDOW* win);

void vittoria(WINDOW* win);

void insertNewScore_menu(int score, int diff);

/* Fine Definizione Libreria */
#endif
