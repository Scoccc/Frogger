/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "timer.h"

void* start_timer(void* unlimited_time)
{
	
    /* Sezione Dichiarativa */
    Timer timer;
    Packet pck;
    int *trucco = (int*) unlimited_time;

    /* Sezione Esecutiva */
    /* Imposto il pacchetto timer */
    timer.tempo_rimasto = TIME;

    /* Ciclo di esecuzione */
    while (timer.tempo_rimasto >= 0) {

        /* Impacchetto */
        pck = pack_timer(timer);

        /* Invio */
		push(&lista, pck, &semaforo);

        /* Decremento il tempo */
        if (*trucco == 0) timer.tempo_rimasto = timer.tempo_rimasto - 1;

        /* Attendo 1 secondo */
        sleep(1);

    }

    /* Uccido il processo */
    _exit(1);

}

Packet pack_timer(Timer t) {

    Packet pck;

    pck.identifier = TIMER_ID;
    pck.timer.tempo_rimasto = t.tempo_rimasto;
    
    return pck;

}
