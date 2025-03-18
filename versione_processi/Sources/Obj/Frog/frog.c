/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "frog.h"

void start_frog(int fd_out[2])
{	
	Packet pkt;
	Frog frog;
	int input;
	
	close(fd_out[READ_END]);
	
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	
	while(1)
	{	
		input = getch();

		pkt = pack_input(input);
		write(fd_out[WRITE_END],&pkt,sizeof(Packet));		
			
	}
	
	close(fd_out[WRITE_END]);
}

Packet pack_input(int input)
{
	Packet pck;
	
	pck.identifier = FROG_ID;
	pck.input = input;
	
	return pck;
}
