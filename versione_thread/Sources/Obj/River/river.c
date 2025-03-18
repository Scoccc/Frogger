/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "river.h"

/* Definizione Prototipi */
void* start_log(void *thread_argument)
{

    	/* Sezione Dichiarativa */
		Thread_Log_Initializer *a = (Thread_Log_Initializer *) thread_argument;
		Corrente corrente = a->corrente;
		Difficolta diff = a->diff;
    	Log log;
    	Packet pck, arrivato;
    	pid_t pid_bullet;
	int shot_prob;

    	/* Sezione Esecutiva */
		srand(pthread_self());

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

    	/* Ciclo di Esecuzione */
		while (TRUE) 
		{
			/* Se ha il tronco ha un nemico e becca la probabilità spara */
			log.shoot = log.has_enemy && rollProbability(100, shot_prob);

			/* Comunico al padre la posizione del tronco */
			pck = pack_log(log);
			push(&lista, pck, &semaforo);

			/* Attendo per scoprire se far spawnare un nemico oppure no */
			arrivato = pop(&listeToLogs[corrente], &semaforiLogs[corrente]);
			log = unpack_log(arrivato);

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

Log unpack_log(Packet pkt) 
{
	Log log;

	log.pos.r = pkt.log.pos.r;
	log.pos.c = pkt.log.pos.c;
	log.length = pkt.log.length;
	log.has_enemy = pkt.log.has_enemy;
	log.h_dir = pkt.log.h_dir;
	log.shoot = pkt.log.shoot;

	return log;
}

