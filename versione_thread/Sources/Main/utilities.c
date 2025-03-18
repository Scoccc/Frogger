#include "../../Main/utilities.h"

/* Inizializzazione Variabili Globali */
pthread_mutex_t semaforo = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t semaforoCars = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t semaforiLogs[NUM_LOGS] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER}; 
Queue lista;
Queue listaToCars;
Queue listeToLogs[NUM_LOGS];

/* Inizializzazione Sottoprogrammi */
void refresh_all (WINDOW** frames)
{
	for (int i = 0; i < NUM_FRAMES; i++)
		wrefresh(frames[i]);
}

void clear_all(WINDOW** frames)
{
	for (int i = 0; i < NUM_FRAMES; i++)
		wclear(frames[i]);
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

void fgoto(FILE* file, char* label)
{
	char buff[BUFF_SIZE];
	while(fscanf(file, "%s", buff) == 1)
		if(strcmp(buff, label) == 0)
			return;
}

void clearTiles(Tiles tiles[GAME_H][GAME_W])
{
	for (int i = 2; i <= 6; i++)
		for (int j = 0; j < GAME_W; j++)
			tiles[i][j] = W;
}

void init_hitboxes(Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W])
{
	for (int r = 0; r < MAINFRAME_H; r++)
	{
		for (int c = 0; c < MAINFRAME_W; c++)
		{
			switch(tiles[r/CELL_H][c/CELL_W])
			{
				case G:
				case S:
					hit_boxes[r][c] = SAFE;
					break;
				case W:
					hit_boxes[r][c] = HB_WATER;
					break;
				case T:
					hit_boxes[r][c] = HB_TANA;
					break;
				case X:
					hit_boxes[r][c] = HB_BORDER;
					break;
			}
		}
	}
}

_Bool rollProbability(int tot, int prob)
{
	return rand() % tot < prob;
}

void noCaseSensitive(char *string) {

	for (int cont = 0; cont < strlen(string); cont++) {
		string[cont] = toupper(string[cont]);
	}

}

void push (Queue *lista, Packet new, pthread_mutex_t *sem) {

    /* Sezione Dichiarativa */
    Node *nodo;

    /* Sezione Esecutiva */
	pthread_mutex_lock(sem);

    /* Alloco il nuovo nodo */
    nodo = (Node*) malloc (sizeof(Node));
    nodo->p = new;
    nodo->link = NULL;

    /* Controllo se la coda e' vuota */
    if (isQueueEmpty(lista) == true) {
        lista->testa = nodo;
        lista->coda = nodo;
        lista->dim = 0;
    } else {

        /* Inserisco il nodo nella lista non vuota */
        lista->coda->link = nodo;
        lista->coda = nodo;

    }

    /* Incremento il numero di elementi */
    lista->dim = lista->dim + 1;

	pthread_mutex_unlock(sem);
	sem_post(&(lista->cont));

}

Packet pop (Queue *lista, pthread_mutex_t *sem) {

    /* Sezione Dichiarativa */
    Packet res;
    Node *tmp;

    /* Sezione Esecutiva */
	sem_wait(&(lista->cont));
	pthread_mutex_lock(sem);

    /* Controllo se la lista e' vuota */
    if (isQueueEmpty(lista) == true) {
        return res;
    }

    /* Rimuovo il primo elemento */
    tmp = lista->testa;
    res = lista->testa->p;

    /* Controllo se il nodo tolto era l'ultimo rimasto */
    if (lista->dim == 1) {
        lista->testa = NULL;
        lista->coda = NULL;
    } else {
        lista->testa = lista->testa->link;
    }

    /* Decremento il numero di elementi */
    lista->dim = lista->dim - 1;
    free(tmp);

    /* Dichiarazioni Finali */
	pthread_mutex_unlock(sem);
    return res;
}

bool isQueueEmpty(Queue *lista) {

    /* Sezione Dichiarativa */
    bool vuoto = false;

    /* Sezione Esecutiva */
    if (lista->testa == NULL && lista->coda == NULL) vuoto = true;

    /* Dichiarazioni Finali */
    return vuoto;

}

void init_queue(Queue *lista) {
	lista->testa = NULL;
    lista->coda = NULL;
    lista->dim = 0;
	sem_init(&(lista->cont),0,0);
}