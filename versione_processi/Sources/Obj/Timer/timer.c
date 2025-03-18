/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "timer.h"

void start_timer(int fd[2], int unlimited_time)
{
	
    /* Sezione Dichiarativa */
    Timer timer;
    Packet pck;

    /* Sezione Esecutiva */

    /* Imposto il pacchetto timer */
    timer.tempo_rimasto = TIME;

    /* Chiudo il canale di lettura */
    close(fd[READ_END]);

    /* Ciclo di esecuzione */
    while (timer.tempo_rimasto >= 0) {

        /* Impacchetto */
        pck = pack_timer(timer);

        /* Invio */
        write(fd[WRITE_END], &pck, sizeof(Packet));

        /* Decremento il tempo */
        if (unlimited_time == 0) timer.tempo_rimasto = timer.tempo_rimasto - 1;

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
