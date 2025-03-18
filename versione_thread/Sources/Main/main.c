/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */
 
/* Direttive al Preprocessore */
/* Direttive di Sistema */
#include <ncurses.h>
#include <locale.h>
#include <stdbool.h>

/* Direttive Locali */
#include "../Game/game.h"

/* Programma Principale */
int main()
{

	/* Sezione Dichiarativa */
	Menu selected; //Opzione selezionata dal menu
	_Bool esci = FALSE;
		
	/* Sezione Esecutiva */
	/* Inizializzazione dello schermo per ncurses */
	setlocale(LC_CTYPE,"");			//Permette di usare i caratteri estesi con ncurses		
	initscr();	       				//Prepara il terminal all'utilizzo di ncurses
	start_color();          		//Permette di usare i colori con ncurses
	curs_set(0);            		//Rende il cursore invisibile
	noecho();               		//Rimuove l'eco degli input su terminale
	
	/* Controllo che le dimensioni del terminale siano abbastanza grandi */
	sizes_menu(); 
	
	/* Mostro il titolo di gioco */
	title_menu();
	
	/* Mostro il menu' del gioco */
	do
	{
		/* Mostro all'utente il menu' e permetto una scelta tra le diverse opzioni */
		selected = game_menu();
		
		/* Gestisco la scelta dell'utente */
		switch(selected)
		{
			case NEW_GAME:
				new_game();
				break;

			case VIEW_SCORE:
				view_scores_menu();
				break;

			case OPTIONS:		
				color_change_menu();
				break;

			case SECRET_MENU:
				secret_menu();
				break;
				
			case EXIT:
				esci = true;
				break;
			
			default:
				perror("var <select> not in range\n");
		}
	}
	while(!esci);

	/* Direttive Finali */
	endwin();
	return 0; //Terminazione corretta e uscita dal gioco	
}
