/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "river.h"

/* Definizione Prototipi */
void start_log(int fd_out[2], int fd_in[2], Corrente corrente, Difficolta diff)
{

    	/* Sezione Dichiarativa */
    	Log log;
    	Packet pck;
    	pid_t pid_bullet;
		int shot_prob;

    	/* Sezione Esecutiva */
		srand(getpid());

   		/* Imposto le variabili iniziali del tronco */
    	log.length = rand() % (MAX_LOG_W - MIN_LOG_W) + MIN_LOG_W;
    	log.pos.c = rand() % MAINFRAME_W - log.length;
    	log.has_enemy = rand() % 2; 
    	log.h_dir = rand() % 2 == 0 ? RIGHT : LEFT;
    	log.shoot = FALSE;

    	/* Imposto in quale corrente il tronco deve stare */
    	switch (corrente)
    	{
        	case CORRENTE1:
		    	log.pos.r = CORRENTEFIUME1_R;
		    	break;
			case CORRENTE2:
		    	log.pos.r = CORRENTEFIUME2_R;
		    	break;
			case CORRENTE3:
		    	log.pos.r = CORRENTEFIUME3_R;
		    	break;
			case CORRENTE4:
		    	log.pos.r = CORRENTEFIUME4_R;
		    	break;
			case CORRENTE5:
		    	log.pos.r = CORRENTEFIUME5_R;
		    	break;
    	}

		/* La possibilita' di sparare varia in base alla difficolta' */
		switch (diff) {
			case Facile:
				shot_prob = 0;
				break;
			case Normale:
				shot_prob = 1;
				break;
			case Difficile:
				shot_prob = 3;
				break;
			case Impossibile:
				shot_prob = 5;
				break;
		}

		/* Chiudo i canali di comunicazione non utilizzati dei pipe */
		close(fd_out[READ_END]);
		close(fd_in[WRITE_END]);

    	/* Ciclo di Esecuzione */
		while (TRUE) 
		{
			/* Se ha il tronco ha un nemico e becca la probabilità spara */
			log.shoot = log.has_enemy && rollProbability(100, shot_prob);

			/* Comunico al padre la posizione del tronco */
			pck = pack_log(log);
			write(fd_out[WRITE_END], &pck, sizeof(Packet));

			/* Attendo per scoprire se far spawnare un nemico oppure no */
			read(fd_in[READ_END], &log.has_enemy, sizeof(_Bool));

			/* Gestisco lo spostamento orizzontale con rimbalzi */
			if (log.pos.c >= MAINFRAME_W - log.length) log.h_dir = LEFT;
			if (log.pos.c <= 0) log.h_dir = RIGHT;
			
			/* Incremento lo spostamento della rana orizzontalmente */
			log.pos.c += log.h_dir; 

			/* Resetto la probabilita' dello sparo */
			log.shoot = FALSE; 
			
			/* Attendo */
			usleep(LOG_DELAY);
		}

		/* Chiudo i pipe di communicazione */
		close(fd_out[WRITE_END]);
		close(fd_in[READ_END]);
}

Packet pack_log(Log log) 
{
    Packet pkt;
    pkt.identifier = LOG_ID;
    pkt.log.pos.r = log.pos.r;
    pkt.log.pos.c = log.pos.c;
    pkt.log.length = log.length;
    pkt.log.has_enemy = log.has_enemy;
    pkt.log.h_dir = log.h_dir;
    pkt.log.shoot = log.shoot;
    
    return pkt;
}
