/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "frog.h"

void* start_frog()
{	
	Packet pkt;
	Frog frog;
	int input;

	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	
	while(1)
	{	
		input = getch();

		pkt = pack_input(input);

		push(&lista, pkt, &semaforo);
			
	}
	

}

Packet pack_input(int input)
{
	Packet pck;
	
	pck.identifier = FROG_ID;
	pck.input = input;
	
	return pck;
}
