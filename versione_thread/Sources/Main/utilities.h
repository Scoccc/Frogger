/* * * * * * * * * * * * * * * * * * * * 
 *                FROGGER              *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <ncurses.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>

/* Inizio Definizioni Libreria */
#ifndef UTILITIES
#define UTILITIES

/* Inizializzazione Costanti */
#define BUFF_SIZE 100
#define DELAY 10000
#define MAX_NAME_SIZE 20

/* Parametri della Rana */
#define FROG_W 4
#define FROG_H 2

/* ERRORI */
#define ERR_FROG_FAILED         -1 
#define ERR_STREET_FAILED       -2 
#define ERR_RIVER_FAcILED       -3
#define ERR_TIMER_FAILED        -4
#define ERR_PIPE_FAILED         -5
#define ERR_CAR_FAILED          -6
#define ERR_LOG_FAILED	        -8
#define ERR_MENU_COLORS_FAILED  -9
#define ERR_BULLET_FAILED       -10
#define ERR_SECRET_MENU_LOADING -11
#define ERR_NEW_GAME_LOADING    -12
#define VIEW_SCORES_FAILED      -13
#define INSERT_SCORES_FAILED    -14

/* Dimensioni Cella */
#define CELL_W 4 
#define CELL_H 2

/* Dimensioni Logiche */
#define GAME_W 15
#define GAME_H 14

/* Dimensioni Reali Finestre */
#define TITLE_W                        59
#define TITLE_H                         9
#define MENU_W                         30
#define MENU_H                          9
#define SECRET_MENU_W                  50
#define SECRET_MENU_H                   10
#define MAINFRAME_W     (GAME_W * CELL_W)
#define MAINFRAME_H     (GAME_H * CELL_H)
#define HP_W                           11
#define HP_H                            3
#define TIMER_W (MAINFRAME_W - HP_W - 16)
#define TIMER_H                      HP_H
#define SCORE_W                        15
#define SCORE_H                         3
#define BULLETS_W                      11
#define BULLETS_H                 SCORE_H
#define POWER_W   (MAINFRAME_W - BULLETS_W - SCORE_W - 2)
#define POWER_H                   SCORE_H
#define COLOR_MENU_W                   40
#define COLOR_MENU_H                   34
#define INFO_W                         38
#define INFO_H                         23
#define INSERT_SCORE_W                 28
#define INSERT_SCORE_H                 7
#define VIEW_SCORE_MENU_W              51
#define VIEW_SCORE_MENU_H              34

/* Costanti del menu' */
#define MAX_MENU 5

/* Altre Costanti */
#define TIME 60 //tempo di gioco in secondi
#define SOGLIA_GIALLO (50 * TIME / 100) //50% del tempo
#define SOGLIA_ROSSO (25 * TIME / 100) //25% del tempo
#define MIN_R SCORE_H + MAINFRAME_H + TIMER_H//dimensionw minima verticale del terminale
#define MIN_C MAINFRAME_W //dimensionw minima orizzontale del terminale
#define INVIO '\n'
#define NUM_TANE 5
#define MAX_BULLETS_NUM 20 //Numero masismo di proiettili su schermo
#define ALLERT_OFFSET 20

/* Colori */
#define RGB_COLOR           8
#define TIMER_COLOR         9
#define FROG_COLOR         10
#define FROG_EYES_COLOR    11
#define GRASS_COLOR_1      12
#define GRASS_COLOR_2      13
#define STREET_COLOR_1     14
#define STREET_COLOR_2     15
#define WATER_COLOR_1      16
#define WATER_COLOR_2      17
#define LOG_COLOR_1        18
#define TIRE_COLOR         19
#define CAR1_COLOR         20
#define CAR2_COLOR         21
#define CAR3_COLOR         22
#define CAR4_COLOR         23
#define CARICO_COLOR       24
#define CAR5_COLOR1        25
#define GLASS_COLOR        26
#define DETAILS_COLOR      27
#define CAR5_COLOR2        28
#define ENEMY_COLOR        29
#define ENEMY_BULLET_COLOR 30
#define FROG_BULLET_COLOR  31
#define TANE_COLOR_1	   32
#define TANE_COLOR_2	   33
#define PLANTS_COLOR       34
#define MENU_MIXED_COLOR   35
#define BULLETS_MENU_COLOR 36
#define POWER_COLOR        37
#define FROG_ARMOR_COLOR   38

/* Set di Colori */
#define BW_PAIR            1
#define FROG_PAIR_1        2
#define FROG_PAIR_2        3
#define GRASS_PAIR         4
#define STREET_PAIR        5
#define WATER_PAIR         6
#define TANA_PAIR          7
#define BORDER_PAIR        8
#define TIMER_PAIR         9
#define CAR1_PAIR_1       10
#define CAR1_PAIR_2       11
#define CAR1_PAIR_3       12
#define CAR1_PAIR_4       13
#define CAR2_PAIR_1       14
#define CAR2_PAIR_2       15
#define CAR2_PAIR_3       16
#define CAR2_PAIR_4       17
#define CAR3_PAIR_1       18
#define CAR3_PAIR_2       19
#define CAR3_PAIR_3       20
#define CAR3_PAIR_4       21
#define CAR4_PAIR_1       22
#define CAR4_PAIR_2       23
#define CAR4_PAIR_3       24
#define CAR5_PAIR_1       25
#define CAR5_PAIR_2       26
#define CAR5_PAIR_3       27
#define LOG_PAIR_1  	  28
#define LOG_PAIR_2        29
#define ENEMY_PAIR1       30
#define ENEMY_PAIR2       31
#define E_BULLET_G_PAIR   32
#define F_BULLET_G_PAIR   33
#define E_BULLET_S_PAIR   34
#define F_BULLET_S_PAIR   35
#define E_BULLET_W_PAIR   36
#define F_BULLET_W_PAIR   37
#define E_BULLET_L_PAIR   38
#define F_BULLET_L_PAIR   39
#define F_BULLET_T_PAIR   40
#define E_BULLET_T_PAIR   41
#define F_BULLET_X_PAIR   42
#define E_BULLET_X_PAIR   43
#define MENU_PAIR         44
#define ALLERT_PAIR       45
#define BULLETS_MENU_PAIR 47
#define POWER_PAIR        48
#define RGB_PAIR          49
#define FROG_ARMOR_PAIR   50

#define NUM_MENU_PAIRS 25

/* Costanti relative al file "options.ini" */
#define DIM_LINEA  100 	//Numero massimo di caratteri da leggere in una riga di file
#define DIM_COLORS  30 	//Numero di righe utilizzate per i colori
#define DIM_LIVES    1	//Numero di righe utilizzate per le vite
#define MAX_LIVES    3	//Numero massimo di vite per la rana
#define MIN_LIVES    1	//Numero minimo di vite per la rana

/* Dimensioni Macchine */
#define CAR1_W  8
#define CAR2_W  8
#define CAR3_W  8
#define CAR4_W  9
#define CAR5_W 17

/* Costanti relative alle corsie */
#define NUM_CORSIE      5
#define MAX_CAR_OFFSET  30

#define CORSIA1_R (CELL_H * 12)
#define CORSIA2_R (CELL_H * 11)
#define CORSIA3_R (CELL_H * 10)
#define CORSIA4_R (CELL_H * 9)
#define CORSIA5_R (CELL_H * 8)

#define CAR1_SPEED 5
#define CAR2_SPEED 2
#define CAR3_SPEED 3
#define CAR4_SPEED 6
#define CAR5_SPEED 8

/* Tronchi */
#define NUM_LOGS 5
#define MIN_LOG_W (FROG_W * 2)
#define MAX_LOG_W (FROG_W * 3)
#define LOG_DELAY 100000
#define MIN_ENEMY_SPAWN_DELAY 200
#define MAX_ENEMY_SPAWN_DELAY 1000
#define CORRENTEFIUME1_R (CELL_H*2)
#define CORRENTEFIUME2_R (CELL_H*3)
#define CORRENTEFIUME3_R (CELL_H*4)
#define CORRENTEFIUME4_R (CELL_H*5)
#define CORRENTEFIUME5_R (CELL_H*6)
#define SPAWN_RATE_NEMICO 1

/* BULLETS */
#define MAX_BULLET_SPEED 10
#define RECOIL 50
#define NUM_FROG_BULLETS 5
#define NUM_ENEMY_BULLETS 15
#define FREE_BULLET -1
#define LOST_BULLET -2
#define MAX_POWER 80
#define ENEMY_BULLET_SPEED 2

/* Gestione delle vite della rana */
#define MIN_FROG_HP 0
#define MAX_FROG_HP 3

/* Gestione dello score */
#define MIN_SCORE     0
#define MAX_SCORE 99999

#define SPAZIO_MINIMO_TRA_MACCHINE 4

#define NUM_FRAMES 6

/* HUD scelta difficolta' */
#define MAX_DIFF 4
#define MIN_DIFF 0

/* Costanti relative alla difficolta' */
#define NUM_CARS_FACILE 3
#define NUM_CARS_NORMALE 5
#define NUM_CARS_DIFFICILE 7
#define NUM_CARS_IMPOSSIBILE 14

/* Costanti relative ai cheats code */
#define NUM_CHEATS 6
#define NUM_STRING 30

/* CTRL + <TASTO> */
#define CTRL(x) ((x) & 0x1f)

/* Definizione enumerazioni */
typedef enum {NEW_GAME, VIEW_SCORE, OPTIONS, SECRET_MENU, EXIT} Menu;

typedef enum {FROG_ID, CAR_ID, LOG_ID, BULLET_ID, TIMER_ID, NOTHING} Identifier;

typedef enum {CAR1, CAR2, CAR3, CAR4, CAR5, WAITING_CAR} CarTypes;

typedef enum {CORSIA1, CORSIA2, CORSIA3, CORSIA4, CORSIA5, NULL_CORSIA} Corsie;

typedef enum {CORRENTE1 = 0, CORRENTE2, CORRENTE3, CORRENTE4, CORRENTE5} Corrente;

typedef enum {MAINFRAME, TIMER, HP, SCORE, BULLETS, POWER} Frames;

typedef enum {SQUEEZED, DROWNED, SHOT, TIME_END, FILLED, NOT_EXIT} Exit_status;

typedef enum {SAFE, HB_FROG, HB_CAR, HB_ENEMY, HB_TANA, HB_WATER, HB_BULLET, HB_BORDER} HitBoxes;

typedef enum {LEFT = -1, STILL = 0, RIGHT = 1} H_Dir; //Direzione Orizzontale

typedef enum {UP = -1, DOWN = 1} V_Dir;

typedef enum {Facile = 0, Normale, Difficile, Impossibile} Difficolta;

typedef enum {UPPER_BORDER, BOTTOM_BORDER, HIT_FROG, HIT_ENEMY, HIT_CAR, BULLET_CLASH, NO_HIT} Bullet_Collision_Status;

typedef enum {UNLIMITED_HP, UNLIMITED_TIME, NO_CAR_COLLISION, NO_BULLET_COLLISION, RAINBOW_FROG, ALTERNATIVE_GRAPHICS} Chaet;

/*
 * Le Tiles permettono di disegnare il colore di sfondo degli
 * oggetti che si sovrappongono ad una superfice.
 */
typedef enum 
{
	W, //Water
	G, //Grass
	S, //Street
	T, //Tana
	L, //Log
	X  //Border
} Tiles;

typedef enum {R=0,/*G*/ B=2} Colors; //G è già definito sù come 1 e va bene così, evviva il riciclo

/* Struttura Posizione
   Abbiamo preferito sostituire (y,x) con (r,c) per semplicita' nostra di lettura */
typedef struct
{
	int r; //riga
	int c; //colonna
}Pos;

typedef struct
{
	Pos pos;
	Pos prev;
}Frog;

typedef struct
{
	Pos pos;
	int speed;
	H_Dir h_dir;
	int width;
	CarTypes car_type;
}Car;

typedef struct
{
	Pos pos;
	int length;
	_Bool has_enemy;
	_Bool shoot;
	H_Dir h_dir;
}Log;

typedef struct
{
	pthread_t tid;
	Pos pos;
	V_Dir dir; 
	int speed;
	_Bool starting;
}Bullet;

typedef struct
{
	int tempo_rimasto;
}Timer;

typedef struct
{
	Identifier identifier;
	union
	{
		int input;
		Car car;
		Log log;
		Bullet bullet;
		Timer timer;
	};
}Packet;

typedef struct {
	char code[NUM_STRING];
	int working;
} Cheat_Code;

typedef struct node {
    struct node *link;
    Packet p;
} Node;

typedef struct {
    Node *testa;
    Node *coda;
    int dim;
	sem_t cont;
} Queue;

typedef struct {
	Corrente corrente;
	Difficolta diff;
} Thread_Log_Initializer;

/* Dichiarazione Variabili Globali */
extern pthread_mutex_t semaforo, semaforoCars, semaforiLogs[NUM_LOGS];
extern Queue lista, listaToCars, listeToLogs[NUM_LOGS];

/* Inizializzazione Sottoprogrammi */
void refresh_all(WINDOW** frames);

void clear_all(WINDOW** frames);

int max(int a, int b);

int min(int a, int b);

void fgoto(FILE* file, char* label); //ricerca label in un file

void clearTiles(Tiles tiles[GAME_H][GAME_W]);

void init_hitboxes(Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W]);

_Bool rollProbability(int tot, int prob);

void noCaseSensitive(char *string);

void push (Queue *lista, Packet new, pthread_mutex_t *sem);

Packet pop (Queue *lista, pthread_mutex_t *sem);

bool isQueueEmpty(Queue *lista);

void init_queue(Queue *lista);

/* Fine Definizione Libreria */
#endif
