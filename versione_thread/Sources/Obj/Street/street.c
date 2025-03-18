/* * * * * * * * * * * * * * * * * * * *
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "street.h"

void* start_car()
{
	Car car;
	Packet pkt, arrivato;

	car.pos.r = CORSIA1_R;
	car.pos.c = -CAR1_W;
	car.h_dir = RIGHT;
	car.speed = CAR1_SPEED;
	car.width = CAR1_W;
	car.car_type = WAITING_CAR;
	
	while(TRUE)
	{	
		if
		(
			car.car_type == WAITING_CAR                      //La macchina è in attesa
			|| (car.h_dir == RIGHT && car.pos.c >= MAINFRAME_W)  //oppure la macchian si spostava verso destra ed è uscita dal bordo destro
			|| (car.h_dir == LEFT && car.pos.c <= -car.width)  //oppure la macchian si spostava verso sinistra ed è uscita dal bordo sinistro
		)
		{					
			car.car_type = WAITING_CAR;

			pkt = pack_car(car);
			push(&lista, pkt, &semaforo);

			arrivato = pop(&listaToCars, &semaforoCars);
			car = unpack_car(arrivato);

			
		}
		else
		{
			
			car.pos.c += car.h_dir; //Muovi la macchina
			
			pkt = pack_car(car); //Impacchetta la macchina
			push(&lista, pkt, &semaforo);
			
			usleep(DELAY * car.speed); //attendi
		}
		
		
	}
	
	exit(1);
}

Packet pack_car(Car car)
{
	Packet pkt;
	pkt.identifier = CAR_ID;
	pkt.car.pos.r = car.pos.r;
	pkt.car.pos.c = car.pos.c;
	pkt.car.h_dir = car.h_dir;
	pkt.car.speed = car.speed;
	pkt.car.width = car.width;
	pkt.car.car_type = car.car_type;
	
	return pkt;
}

Car unpack_car(Packet pkt)
{
	Car car;
	
	car.pos.r = pkt.car.pos.r;
	car.pos.c = pkt.car.pos.c;
	car.h_dir = pkt.car.h_dir;
	car.speed = pkt.car.speed;
	car.width = pkt.car.width;
	car.car_type = pkt.car.car_type;
	
	return car;
}