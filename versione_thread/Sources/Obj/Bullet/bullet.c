/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "bullet.h"

void* start_bullet(void* init_bullet)
{	
	Packet pkt;
	Bullet *b = (Bullet *) init_bullet;
	Bullet bullet = *b;

	pkt = pack_bullet(bullet);
	push(&lista, pkt, &semaforo);
	bullet.starting = FALSE;

	while(1)
	{	
		bullet.pos.r += bullet.dir;
		pkt = pack_bullet(bullet);
		push(&lista, pkt, &semaforo);
		usleep(DELAY * (MAX_BULLET_SPEED - bullet.speed));
	}
	
}

Bullet set_frog_bullet(Frog frog, int charge)
{
	Bullet init_bullet;
	
	init_bullet.dir = UP;
	init_bullet.pos.r = frog.pos.r - 1;
	init_bullet.pos.c = frog.pos.c + 1;
	init_bullet.speed = (charge / 10);
	init_bullet.starting = TRUE; 
	
	return init_bullet;
}

Bullet set_enemy_bullet(Log log, int speed)
{
	Bullet init_bullet;
	
	init_bullet.dir = DOWN;
	init_bullet.pos.r = log.pos.r + CELL_H;
	init_bullet.pos.c = log.pos.c + log.length/ 2;
	init_bullet.speed = speed; 
	init_bullet.starting = TRUE;
	
	return init_bullet;
}

Packet pack_bullet(Bullet bullet)
{
	Packet pck;
	
	pck.bullet.tid = pthread_self();
	pck.identifier = BULLET_ID;
	pck.bullet.pos.r = bullet.pos.r;
	pck.bullet.pos.c = bullet.pos.c;
	pck.bullet.dir = bullet.dir;
	pck.bullet.speed = bullet.speed;
	pck.bullet.starting = bullet.starting;

	return pck;
}

Bullet unpack_bullet(Packet pkt) 
{
	Bullet bullet;

	bullet.tid = pkt.bullet.tid;
	bullet.pos.r = pkt.bullet.pos.r;
	bullet.pos.c = pkt.bullet.pos.c;
	bullet.speed = pkt.bullet.speed;
	bullet.dir = pkt.bullet.dir;
	bullet.starting = pkt.bullet.starting;

	return bullet;
}
