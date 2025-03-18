/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "game.h"

int new_game()
{
	/* Sezione Dichiarativa */
	pid_t pid_frog, pid_timer;
	pid_t pid_cars[60];
	pid_t pid_logs[NUM_LOGS];
	pid_t frog_bullets[NUM_FROG_BULLETS];
	pid_t enemy_bullets[NUM_ENEMY_BULLETS];
	int fd[NUM_PIPES][2]; 
	int max_r, max_c;
	int tane_vuote = NUM_TANE;
	Tiles tiles[GAME_H][GAME_W] = 
   	{	
   		{X,X,X,X,X,X,X,X,X,X,X,X,X,X,X},
   		{X,T,X,X,T,X,X,T,X,X,T,X,X,T,X},
   		{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
   		{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
   		{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
   		{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
   		{W,W,W,W,W,W,W,W,W,W,W,W,W,W,W},
   		{G,G,G,G,G,G,G,G,G,G,G,G,G,G,G},
   		{S,S,S,S,S,S,S,S,S,S,S,S,S,S,S},
   		{S,S,S,S,S,S,S,S,S,S,S,S,S,S,S},
   		{S,S,S,S,S,S,S,S,S,S,S,S,S,S,S},
   		{S,S,S,S,S,S,S,S,S,S,S,S,S,S,S},
   		{S,S,S,S,S,S,S,S,S,S,S,S,S,S,S},
   		{G,G,G,G,G,G,G,G,G,G,G,G,G,G,G}
   	}; //sarebbe interessante farlo modulare
   	int vite = MAX_FROG_HP;
   	int score = 0;
	int cheat_attivi = 0;
   	Exit_status status;	
	Difficolta diff;
	int numcars, cont = 0;
	Cheat_Code codici[NUM_CHEATS];
	FILE *opzioni;
	
	/* Apro il file opzioni e ottengo i possibili cheat code attivati */
	opzioni = fopen("Main/options.ini", "r");
	if (opzioni == NULL) 
	{
		perror("Errore apertura file opzioni.\n");
		exit(ERR_NEW_GAME_LOADING);
	}
	fgoto(opzioni, "[Cheats_Code]");
	while (fscanf(opzioni, "%s : %d", codici[cont].code, &codici[cont].working) == 2) cont++;

	/* Chiudo il file opzioni */
	fclose(opzioni);

	/* Faccio selezionare la difficolta' all'utente */
	diff = scegli_difficolta();
	switch (diff) {
		case Facile:
			numcars = NUM_CARS_FACILE;
			break;
		case Normale:
			numcars = NUM_CARS_NORMALE;
			break;
		case Difficile:
			numcars = NUM_CARS_DIFFICILE;
			break;
		case Impossibile:
			numcars = NUM_CARS_IMPOSSIBILE;
			break;
	}
	if(codici[NO_CAR_COLLISION].working) numcars = 0;

	/* Alloco il vettore di pid_t cars */
   	for (int i = 0; i < numcars; i++)
   		pid_cars[i] = -1;
   		
	/* Ottengo le dimensioni dello schermo */
	getmaxyx(stdscr, max_r, max_c);

	/* Setto che i tasti premuti non compaiano a schermo */
	noecho();
	
	/* Apro tutti i canali pipe */
	for (int i = 0; i < NUM_PIPES; i++)
		if(pipe(fd[i]) == -1) return ERR_PIPE_FAILED;
	
	/* Setto i colori */
	init_colors();
	init_cp(); 

	/* Imposto il seme di generazione casuale */
	srand(time(NULL));
	
	/* Stampo la scritta "TIME" in basso a destra dello schermo di gioco */
	draw_TIME();
	
	/* Creo le finestre per: gioco, timer, hp, score e bullets */
	WINDOW** frames = malloc(sizeof(WINDOW*) * NUM_FRAMES);
	frames[MAINFRAME] = newwin(MAINFRAME_H, MAINFRAME_W, (max_r - MAINFRAME_H) / 2, (max_c - MAINFRAME_W) / 2);
	frames[TIMER] = newwin(TIMER_H, TIMER_W, ((max_r - MAINFRAME_H) / 2) + MAINFRAME_H,((max_c - MAINFRAME_W) / 2) + MAINFRAME_W-TIMER_W-16);
	frames[HP] = newwin(HP_H, HP_W, ((max_r - MAINFRAME_H) / 2) + MAINFRAME_H, (max_c - MAINFRAME_W) / 2);
	frames[SCORE] = newwin(SCORE_H, SCORE_W,  ((max_r - MAINFRAME_H) / 2) - SCORE_H , (max_c - MAINFRAME_W) / 2);
	frames[POWER] = newwin(POWER_H, POWER_W,  ((max_r - MAINFRAME_H) / 2) - POWER_H , ((max_c - MAINFRAME_W) / 2) + SCORE_W + 1);
	frames[BULLETS] = newwin(BULLETS_H, BULLETS_W, ((max_r - MAINFRAME_H) / 2) - SCORE_H, ((max_c - MAINFRAME_W) / 2) + MAINFRAME_W - BULLETS_W);
	
	/* CICLO MANCHE */
	do
	{	
		/* Pulisco l'intero schermo */
		clear_all(frames);
		
		/* Disegno nuovamente le set_enemy_bullettiles */
		box(frames[TIMER],ACS_VLINE,ACS_HLINE);
		box(frames[HP],ACS_VLINE,ACS_HLINE);
		box(frames[BULLETS],ACS_VLINE,ACS_HLINE);
		for (int i = 2; i < BULLETS_W-2; i += 2)
		{
			mvwprintw(frames[BULLETS], 0, i, "┬");
			mvwprintw(frames[BULLETS], 1, i, "│");
			mvwprintw(frames[BULLETS], 2, i, "┴");
		}
		box(frames[SCORE],ACS_VLINE,ACS_HLINE);	
		box(frames[POWER],ACS_VLINE,ACS_HLINE);	
		
		/* Ripulisce le tiles d'acqua */
		clearTiles(tiles);

		draw_tiles(frames[MAINFRAME],tiles); 
		
		/* Aggiorno le varie finestre */
		refresh_all(frames);
		
		/* Inizializzo il processo rana */
		pid_frog = fork();
		if(pid_frog < 0) 
			exit(ERR_FROG_FAILED);
		else if(pid_frog == 0)
			start_frog(fd[COMMON]);

		/* Inizializzo tutti i tronchi */
		for (int i = 0; i < NUM_LOGS; i++) {
			pid_logs[i] = fork();
			if (pid_logs[i] < 0) exit(ERR_LOG_FAILED);
			else if (pid_logs[i] == 0) {
				start_log(fd[COMMON], fd[TO_LOG_1 + i], (Corrente) i, diff);
			}
		}
		
		/* Inizializzo tutte le macchine */
		for (int i = 0; i < numcars; i++)
		{
			pid_cars[i] = fork();
			
			if(pid_cars[i] < 0) 
				exit(ERR_CAR_FAILED);
			else if(pid_cars[i] == 0)
			{
				start_car(fd[COMMON], fd[TO_CAR]);	
			}
		}
		
		/* Inizializzo il processo timer */
		pid_timer = fork();
		if(pid_timer < 0) 
			exit(ERR_TIMER_FAILED);
		else if(pid_timer == 0)
			start_timer(fd[COMMON], codici[UNLIMITED_TIME].working);
			
		/* Faccio partire la manche e recupero il suo exit status */
		status = start_gameloop(fd, frames, tiles, vite, &score, diff, frog_bullets, enemy_bullets, codici, pid_logs);	

		/* 
		 * L'uscita dalla manche puo' derivare da diversi fattori:
		 *  - Squeezed: la rana e' stata schiacciata dalle auto, per questo perde una vita
		 *  - Drowned: la rana e' affogata entrando nel fiume, per questo perde una vita
		 *  - Shot: la rana e' stata colpita da un proiettile o esce dal bordo o colpisce un nemico, per questo perde una vita
		 *  - Time_end: il tempo e' finito, la rana perde una vita
		 *  - Filled: la rana ha raggiunto con successo una tana
		 */
		switch(status)
		{
			case SQUEEZED:
				score = max(0, score - 200);
				if (codici[UNLIMITED_HP].working == 0) vite--;
				break;
			case DROWNED:
				score = max(0, score - 200);
				if (codici[UNLIMITED_HP].working == 0) vite--;
				break;
			case SHOT:
				score = max(0, score - 200);
				if (codici[UNLIMITED_HP].working == 0) vite--;
				break;
			case TIME_END:
				score = max(0, score - 200);
				if (codici[UNLIMITED_HP].working == 0) vite--;
				break;
			case FILLED:
				score = min(MAX_SCORE, score + 500);
				tane_vuote--;
				break;
		}
		draw_score(frames[SCORE], score);
		/* Uccido il processo rana */
		kill(pid_frog, SIGKILL);

		/* Uccido tutti i processi macchine */
		for (int i = 0; i < numcars; i += 1)
			kill(pid_cars[i], SIGKILL);	

		/* Uccido tutti i processi tronchi */
		for (int i = 0; i < NUM_LOGS; i++) 
			kill(pid_logs[i], SIGKILL);
		
		/* Uccido il processo timer */
		kill(pid_timer, SIGKILL);

		/* Uccido tutti i processi proiettili della rana */
		for (int i = 0; i < NUM_FROG_BULLETS; i++)
			if (frog_bullets[i] != FREE_BULLET && frog_bullets[i] != LOST_BULLET) kill(frog_bullets[i], SIGKILL);

		/* Uccido tutti i processi proiettili dei nemici */
		for (int i = 0; i < NUM_ENEMY_BULLETS; i++) {
			if (enemy_bullets[i] != FREE_BULLET) {
				kill(enemy_bullets[i], SIGKILL);
			}
		}
		
		/* Direttive Finali */
		if(vite == MIN_FROG_HP)
		{
			game_over(frames[MAINFRAME]);
		}
		if(tane_vuote == 0)
		{
			vittoria(frames[MAINFRAME]);
			for (int i = 0; i < NUM_CHEATS; i++) cheat_attivi += codici[i].working;
			if (cheat_attivi == 0) insertNewScore_menu(score, diff);
		}
	}	
	while(vite > MIN_FROG_HP && tane_vuote > 0);

	/* Chiusura delle pipe */
	for (int i = 0; i < NUM_PIPES; i++)
	{
		close(fd[i][WRITE_END]);
		close(fd[i][READ_END]);
	} 

	/* Pulizia dello schermo al Game Over */
	clear_all(frames);
	clear();
	refresh_all(frames);
	refresh();

	/* Chiusura array dinamico di frame */
	free(frames);

}

Exit_status start_gameloop(int fd[NUM_PIPES][2], WINDOW** frames, Tiles tiles[GAME_H][GAME_W], int vite, int* score, Difficolta diff, pid_t frog_bullets[NUM_FROG_BULLETS], pid_t enemy_bullets[NUM_ENEMY_BULLETS], Cheat_Code codici[NUM_CHEATS], pid_t pid_logs[NUM_LOGS])
{	
	/* Sezione Dichiarativa */
	/* TIPO         NOME              		        		DESCRIZIONE */
	Packet          pkt;                                                 // Pacchetto arrivato dalla pipe COMMON
   	Frog            frog;                                                // Oggetto di gioco rana
   	Car             car;                                                 // Contenitore macchina acquisita tramite pipe COMMON
   	Log             log;                                                 // Contenitore tronco acquisita tramite pipe COMMON
   	Bullet          bullet;                                              // Contenitore proiettile acquisita tramite pipe COMMON
   	Corsie 		corsia;                                              // Corsia generata su cui far spawnare una macchina in attesa
	Timer           timer;                                               // Timer di gioco
	int             max_r,                                               // Numero massimo di righe del terminale
                        max_c,                                               // Numero massimo di colonne del termina
                        i,                                                   // Index per i vari cicli for
                        input,                                               // Input acquisito dal processo Input
                        power = 0,                                           // Quanto è stato caricato il prossimo colpo
                        recoil = 0,                                          // Ricarica tra un proiettile e l'altro		
                        spawn_prob,                                          // Probabilità che un nemico compaia 
                        slot,                                                // Tiene traccia del primo slot libero di un caricatore
                        caricatoreRana = NUM_FROG_BULLETS,                   // Conta i proiettili della rana, serve solamente per l'HUD
                        r = 0,
                        g = 1000,
                        b = 0;
	H_Dir           dir;                                                 // Direzione generata casulamente per la prima corsia di macchine
	_Bool           corsia_libera[NUM_CORSIE],                           // Permette di tener traccia di quali corsie sono libere e quali no
                        charging,                                            // TRUE: il proiettile sta caricando, FALSE: carica annullata
                        hitbox_view = codici[ALTERNATIVE_GRAPHICS].working,  // TRUE: gli oggetti di gioca saranno visualizatti tramite le lore hitbox
                        sparato[NUM_LOGS];                                   // Vettore per tenere traccia dei nemici sparati
	Exit_status     ex = NOT_EXIT;                       // Stato con cui terminare la manche corrente
	HitBoxes        hit_boxes[MAINFRAME_H][MAINFRAME_W]; // Matrice delle hitboxes
	pid_t           pid_bullet;							 // Variabile temporanea usata per allocare all'interno dei caricatori proiettili dei pid processo

	//array di pid dei proiettili dei nemici

	/* Imposto che nessun nemico e' stato sparato */
	for (i = 0; i < NUM_LOGS; i++) sparato[i] = FALSE;

	/* Inizializzo la matrice di collisione.
	   All'inizio della manche nella matrice vengono salvate soltanto le posizioni degli oggetti statici.*/
   	init_hitboxes(tiles, hit_boxes);

	/* Inizializzo le corsie libere */
	for (i = 0; i < NUM_CORSIE; i++) corsia_libera[i] = TRUE;

	/* Ottengo le dimensioni dello schermo */
	getmaxyx(stdscr,max_r,max_c);

	/* All'inizio di ogni manche la prima corsia delle auto ha una direzione casuale tra destra e sinistra.
	   Le altre corsie si adattano alla direzione di quest'ultima. */
	dir = rand() % 2 ? RIGHT : LEFT;
	
	/* Inizializzo i valori iniziali della frog:
	   in basso al centro del mainframe. */
	frog.prev.r = -1;
	frog.prev.c = -1;
	frog.pos.c = (MAINFRAME_W - FROG_W) / 2; 
	frog.pos.r = (MAINFRAME_H - FROG_H);
	
	/* Disegno la rana.
	   Per informazioni relative a hitbox_view guardare la sezione dichiarativa di questa funzione. */
	if(!hitbox_view)
		draw_frog(frames[MAINFRAME], frog, tiles, hit_boxes, codici[RAINBOW_FROG].working, codici[NO_BULLET_COLLISION].working);

	/* La funzione frog_collision non solo serve a controllare le collisioni, ma anche per posizionare la rana sulla matrice.
	   In questo caso stiamo mettendo la hitbox della rana. */
	frog_collision(frog, hit_boxes, tiles, codici);

	/* Imposto liberi tutti gli slot di proiettili lanciabili della rana */
	for (i = 0; i < NUM_FROG_BULLETS; i++) frog_bullets[i] = FREE_BULLET;

	/* Imposto liberi tutti gli slot di proiettili lanciabili dai nemici */
	for (i = 0; i < NUM_ENEMY_BULLETS; i++) enemy_bullets[i] = FREE_BULLET;
	
	/* Ciclo di Esecuzione */
	while(TRUE)
	{
		/* Lettura da Pipe Common.
		   Ogni oggetto che si muove invia un pacchetto "Packet" al processo padre.
		   Successivamente il padre decomprime il pacchetto con le apposite funzioni e gestisce lo spostamento e le collisioni. */
		read(fd[COMMON][READ_END], &pkt, sizeof(Packet));

		/* Smistamento dei pacchetti */
		switch(pkt.identifier) 
		{
			case FROG_ID:
				input = unpack_input(pkt);
				
				switch(input)
				{
					case KEY_UP:
					case 'w':
					case 'W':
						if(frog.pos.r >= FROG_H)
						{
	
							frog.prev.r = frog.pos.r;
							frog.prev.c = frog.pos.c;
							
							*score = min(MAX_SCORE, *score + vite);
							if(frog.pos.r == CORRENTEFIUME1_R)
								frog.pos.c = ((frog.pos.c+1)/CELL_W) * CELL_W; //Snap
							frog.pos.r -= FROG_H;
						}
						charging = FALSE;
						break;
					case KEY_DOWN:
					case 's':
					case 'S':
						if(frog.pos.r < MAINFRAME_H - FROG_H)
						{
							frog.prev.r = frog.pos.r;
							frog.prev.c = frog.pos.c;

							*score = min(MAX_SCORE, *score + vite);
							if(frog.pos.r == CORRENTEFIUME5_R)
								frog.pos.c = ((frog.pos.c+1)/CELL_W) * CELL_W; //Snap
							frog.pos.r += FROG_H;
						}
						charging = FALSE;
						break;
					case KEY_LEFT:
					case 'a':
					case 'A':
						if(frog.pos.c >= FROG_W)
						{
							frog.prev.r = frog.pos.r;
							frog.prev.c = frog.pos.c;

							*score = min(MAX_SCORE, *score + vite);
							frog.pos.c -= FROG_W;
						}
						charging = FALSE;
						break;
					case KEY_RIGHT:
					case 'd':
					case 'D':
						if(frog.pos.c < MAINFRAME_W - FROG_W)
						{
							frog.prev.r = frog.pos.r;
							frog.prev.c = frog.pos.c;
							
							*score = min(MAX_SCORE, *score + vite);
							frog.pos.c += FROG_W;
						}
						charging = FALSE;
						break;
					case ' ':
						if(recoil == 0)
						{

							slot = getFirstFreeSlot(frog_bullets, NUM_FROG_BULLETS); //la funzione restituisce -1 se non e' presente nemmeno uno slot libero
							if (slot != -1) {
								
								pid_bullet = fork();
								if (pid_bullet < 0)
									_exit(ERR_BULLET_FAILED);
								else if(pid_bullet == 0)
								{
									Bullet init_bullet = set_frog_bullet(frog, power);	
									hit_boxes[init_bullet.pos.r][init_bullet.pos.c] = HB_BULLET;
									start_bullet(fd[COMMON], init_bullet);	
								}

								/* Inserisco il proiettile nel vettore di proiettili rana */
								frog_bullets[slot] = pid_bullet;

								charging = FALSE;
								recoil = RECOIL;
							}

						}
						break;
					case 'c':
					case 'C':
						power = min(MAX_POWER, power+1);
						charging = TRUE;
						break;
				}
				
				ex = frog_collision(frog, hit_boxes, tiles, codici);
				
				if(ex != NOT_EXIT) //se la rana ha fatto collisione
				{
					if(!hitbox_view)
						draw_frog(frames[MAINFRAME], frog, tiles, hit_boxes, codici[RAINBOW_FROG].working, codici[NO_BULLET_COLLISION].working);
					return ex;
				}
				
				if(power > 0 && !charging)
				{
					power = 0;
				}

				draw_power(frames[POWER], power);

				if(!hitbox_view)
					draw_frog(frames[MAINFRAME], frog, tiles, hit_boxes, codici[RAINBOW_FROG].working, codici[NO_BULLET_COLLISION].working);
				break;

			case CAR_ID:
				car = unpack_car(pkt);

				if(car.car_type == WAITING_CAR)
				{					
					
					if(are_corsie_full(corsia_libera))
					{
						corsia = NULL_CORSIA;
					}
					else
					{
						//genera una nuova posizione tra le corsie libere
						do { corsia = rand() % NUM_CORSIE; } while(!corsia_libera[corsia]);
					}
					
					switch(corsia)
					{
						case CORSIA1:
							car.pos.r = CORSIA1_R;
							car.h_dir = dir;
							car.speed = CAR1_SPEED;
							car.width = CAR1_W;
							car.car_type = CAR1;
							break;
						case CORSIA2:
							car.pos.r = CORSIA2_R;
							car.h_dir = -dir;
							car.speed = CAR2_SPEED;
							car.width = CAR2_W;
							car.car_type = CAR2;
							break;
						case CORSIA3:
							car.pos.r = CORSIA3_R;
							car.h_dir = dir;
							car.speed = CAR3_SPEED;
							car.width = CAR3_W;
							car.car_type = CAR3;
							break;
						case CORSIA4:
							car.pos.r = CORSIA4_R;;
							car.h_dir = -dir;
							car.speed = CAR4_SPEED;
							car.width = CAR4_W;
							car.car_type = CAR4;
							break;
						case CORSIA5:
							car.pos.r = CORSIA5_R;
							car.h_dir = dir;
							car.speed = CAR5_SPEED;
							car.width = CAR5_W;
							car.car_type = CAR5;
							break;
						case NULL_CORSIA:
							car.car_type = WAITING_CAR;
							break;
							
					}
					corsia_libera[corsia] = FALSE;
					
					if(car.h_dir == RIGHT)
						car.pos.c = -car.width - (rand() % MAX_CAR_OFFSET);
					else
						car.pos.c = MAINFRAME_W + (rand() % MAX_CAR_OFFSET);
	
					//manda i nuovi dati alla macchina in attesa
					write(fd[TO_CAR][WRITE_END], &car, sizeof(Car));
				}
				else
				{
					if(car_collision(car, hit_boxes)) {
						if (codici[NO_CAR_COLLISION].working == 0) return SQUEEZED;
					}
					
					/* ALLERT per avvertire che da quella corsia sta per spuntare una macchina */
					if(car.pos.c >= -car.width - ALLERT_OFFSET && car.pos.c <= -car.width && car.h_dir == RIGHT)
					{
						wattron(frames[MAINFRAME], COLOR_PAIR(ALLERT_PAIR));
						mvwprintw(frames[MAINFRAME], car.pos.r,     0, "┃");
						mvwprintw(frames[MAINFRAME], car.pos.r + 1, 0, "•");
					}	
					if(car.pos.c >= MAINFRAME_W && car.pos.c <= MAINFRAME_W + ALLERT_OFFSET && car.h_dir == LEFT)
					{
						wattron(frames[MAINFRAME], COLOR_PAIR(ALLERT_PAIR));
						mvwprintw(frames[MAINFRAME], car.pos.r,     MAINFRAME_W - 1, "┃");
						mvwprintw(frames[MAINFRAME], car.pos.r + 1, MAINFRAME_W - 1, "•");
					}
					if(!hitbox_view)
						draw_car(frames[MAINFRAME], car);
				}
				
				//Se la macchina è appena spuntata completamente da destra o da sinistra
				if((car.h_dir == RIGHT && car.pos.c == SPAZIO_MINIMO_TRA_MACCHINE) 
				|| (car.h_dir == LEFT && car.pos.c == MAINFRAME_W - car.width - 1 - SPAZIO_MINIMO_TRA_MACCHINE))
				{
					     if(car.pos.r == CORSIA1_R) corsia_libera[CORSIA1] = TRUE;
					else if(car.pos.r == CORSIA2_R) corsia_libera[CORSIA2] = TRUE;
					else if(car.pos.r == CORSIA3_R) corsia_libera[CORSIA3] = TRUE;
					else if(car.pos.r == CORSIA4_R) corsia_libera[CORSIA4] = TRUE;
					else if(car.pos.r == CORSIA5_R) corsia_libera[CORSIA5] = TRUE;						
				}
				
				break;

			case LOG_ID:
				log = unpack_log(pkt);
				
				for (int cont = 0; cont < NUM_LOGS; cont++) {
					if (sparato[cont] == true) {
						switch (cont) {
							case 0:
								kill(pid_logs[CORRENTE1], SIGKILL);
								break;
								
							case 1:
								kill(pid_logs[CORRENTE2], SIGKILL);
								break;
								
							case 2:
								kill(pid_logs[CORRENTE3], SIGKILL);
								break;
								
							case 3:
							   	kill(pid_logs[CORRENTE4], SIGKILL);
								break;
								
							case 4:
								kill(pid_logs[CORRENTE5], SIGKILL);
								break;

					
						}
					}
				}
				
				
				/* Se il tronco desidera sparare */
				if(log.shoot)
				{
					/* Controllo che ci sia uno slot proiettile libero nel caricatore dei nemici */
					slot = getFirstFreeSlot(enemy_bullets, NUM_ENEMY_BULLETS);
					if (slot != -1) {
						
						/* Creo il processo */
						pid_bullet = fork();
						if (pid_bullet < 0)
							_exit(ERR_BULLET_FAILED);
						else if(pid_bullet == 0)
						{
							Bullet init_bullet = set_enemy_bullet(log, ENEMY_BULLET_SPEED);
							hit_boxes[init_bullet.pos.r][init_bullet.pos.c] = HB_BULLET;
							start_bullet(fd[COMMON], init_bullet);	
						}

					}

					/* Inserisco il proiettile nel caricatore dei nemici */
					enemy_bullets[slot] = pid_bullet;

				} 
				
				for (i = 0; i < GAME_W; i += 1) tiles[log.pos.r / CELL_H][i] = W;
				for (i = log.pos.c; i < log.pos.c + log.length; i++) tiles[log.pos.r / CELL_H][i / CELL_W] = L;
				
				if(log_collision(log, hit_boxes)) return SHOT;
		
				if(!hitbox_view)
					draw_log(frames[MAINFRAME], log);

				if(frog.pos.r == log.pos.r)
				{
					frog.prev.r = -1;
					frog.prev.c = -1;
					frog.pos.c += log.h_dir;
					
					ex = frog_collision(frog, hit_boxes, tiles, codici);
					if(!hitbox_view)
						draw_frog(frames[MAINFRAME], frog, tiles, hit_boxes, codici[RAINBOW_FROG].working, codici[NO_BULLET_COLLISION].working);
				}

				/* Se il tronco non possiede un nemico, vi e' una possibilita' che esso compaia */
				if(log.has_enemy)
				{
					/* Controllo se il nemico e' stato sparato.
					   In quel caso, anche se il suo spawn e' true, deve tornare a false. */
					switch(log.pos.r) {
						case CORRENTEFIUME1_R:
							if (sparato[CORRENTE1]) {
								log.has_enemy = FALSE;
								sparato[CORRENTE1] = FALSE;
							}
							break;
						case CORRENTEFIUME2_R:
							if (sparato[CORRENTE2]) {
								log.has_enemy = FALSE;
								sparato[CORRENTE2] = FALSE;
							}
							break;
						case CORRENTEFIUME3_R:
							if (sparato[CORRENTE3]) {
								log.has_enemy = FALSE;
								sparato[CORRENTE3] = FALSE;
							}
							break;
						case CORRENTEFIUME4_R:
							if (sparato[CORRENTE4]) {
								log.has_enemy = FALSE;
								sparato[CORRENTE4] = FALSE;
							}
							break;
						case CORRENTEFIUME5_R:
							if (sparato[CORRENTE5]) {
								log.has_enemy = FALSE;
								sparato[CORRENTE5] = FALSE;
							}
							break;
					}		
				}
				else
				{
					/* A seconda della difficolta' la probabilita' di comparire cambia */
					switch (diff) {
						case Facile:
							spawn_prob = 0;
							break;
						case Normale:
							spawn_prob = SPAWN_RATE_NEMICO;
							break;
						case Difficile:
							spawn_prob = SPAWN_RATE_NEMICO * 2;
							break;
						case Impossibile:
							spawn_prob = SPAWN_RATE_NEMICO * 3;
							break;
					}
					
					log.has_enemy = frog.pos.r != log.pos.r && rollProbability(100, spawn_prob);
				}
				
				/* Indirizza lo spawn generato sulla giusta pipe.
				   Sia che il nemico debba comparire o no, rimando il il pacchetto al figlio. 
				   Ogni tronco possiede una sua pipe di communicazione. */
				log_router(fd, log.has_enemy, log);
				
				break;

			case BULLET_ID:
				
				bullet = unpack_bullet(pkt);

				/* Controllo le collisioni del proiettile */
				switch (bullet_collision(bullet, hit_boxes, tiles, sparato)) {

					case UPPER_BORDER:
						slot = researchBullet(frog_bullets, NUM_FROG_BULLETS, bullet.pid);
						frog_bullets[slot] = FREE_BULLET;
						kill(bullet.pid, SIGKILL);
						*score = max(MIN_SCORE, *score -  25);
						break;

					case BOTTOM_BORDER:
						slot = researchBullet(enemy_bullets, NUM_ENEMY_BULLETS, bullet.pid);
						enemy_bullets[slot] = FREE_BULLET;
						kill(bullet.pid, SIGKILL);
						break;

					case HIT_FROG:
						hit_boxes[bullet.pos.r - bullet.dir][bullet.pos.c] = SAFE;
						if (codici[NO_BULLET_COLLISION].working == 0) return SHOT;
						break;

					case HIT_ENEMY:
						slot = researchBullet(frog_bullets, NUM_FROG_BULLETS, bullet.pid);
						frog_bullets[slot] = FREE_BULLET;
						kill(bullet.pid, SIGKILL);
						*score = min(MAX_SCORE, *score + 100);
						break;

					case HIT_CAR:
						if (bullet.dir == UP) {
							slot = researchBullet(frog_bullets, NUM_FROG_BULLETS, bullet.pid);
							frog_bullets[slot] = FREE_BULLET;
							kill(bullet.pid, SIGKILL);
							*score = max(MIN_SCORE, *score -  25);
						} else {
							slot = researchBullet(enemy_bullets, NUM_ENEMY_BULLETS, bullet.pid);
							enemy_bullets[slot] = FREE_BULLET;
							kill(bullet.pid, SIGKILL);
						}
						
						hit_boxes[bullet.pos.r - bullet.dir][bullet.pos.c] = SAFE;
						
						break;

					case BULLET_CLASH:
						if (bullet.dir == UP) {
							slot = researchBullet(frog_bullets, NUM_FROG_BULLETS, bullet.pid);
							frog_bullets[slot] = FREE_BULLET;
							kill(bullet.pid, SIGKILL);
							for (i = 0; i < NUM_ENEMY_BULLETS; i++) {
								if (enemy_bullets[i] > 0 && hit_boxes[bullet.pos.r][bullet.pos.c] != HB_BULLET) {
									kill(enemy_bullets[i], SIGKILL);
									enemy_bullets[i] = FREE_BULLET;
								}
							}
						} else {
							slot = researchBullet(enemy_bullets, NUM_ENEMY_BULLETS, bullet.pid);
							enemy_bullets[slot] = FREE_BULLET;
							kill(bullet.pid, SIGKILL);
							for (i = 0; i < NUM_FROG_BULLETS; i++) {
								if (frog_bullets[i] > 0 && hit_boxes[bullet.pos.r][bullet.pos.c] != HB_BULLET) {
									kill(frog_bullets[i], SIGKILL);
									frog_bullets[i] = FREE_BULLET;
								}
							}
						}
						break;

					case NO_HIT:
						draw_bullet(frames[MAINFRAME], bullet, tiles, hit_boxes);
						break;
				}

				/* Se il proiettile e' stato ucciso, bisogna cancellarlo anche dallo schermo */
				if(hit_boxes[bullet.pos.r][bullet.pos.c] != HB_BULLET) 
				{
					switch(tiles[(bullet.pos.r - bullet.dir)/ CELL_H][bullet.pos.c / CELL_W])
					{
						case X:
							wattron(frames[MAINFRAME], COLOR_PAIR(BORDER_PAIR));
							if((bullet.pos.c % CELL_W == 0 && bullet.pos.r - bullet.dir % CELL_H == 0) ||
							  (bullet.pos.c % CELL_W == 2 && bullet.pos.r - bullet.dir % CELL_H == 1))
								mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, "✜");
							else
								mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, " ");
							break;
						case G:
							wattron(frames[MAINFRAME], COLOR_PAIR(GRASS_PAIR));
							mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, "▞");				
							break;
						case S:
							wattron(frames[MAINFRAME], COLOR_PAIR(STREET_PAIR));
							mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, "▗");
							break;
						case T:
						case W:
							wattron(frames[MAINFRAME], COLOR_PAIR(WATER_PAIR));
							mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, "▔");
							break;
						case L:
							wattron(frames[MAINFRAME], COLOR_PAIR(LOG_PAIR_1));
							mvwprintw(frames[MAINFRAME], bullet.pos.r - bullet.dir, bullet.pos.c, "█");
							break;	
					}
				}

				break;

			case TIMER_ID:
				timer = unpack_timer(pkt);
				draw_timer(frames[TIMER], timer, codici[UNLIMITED_TIME].working);
				if (timer.tempo_rimasto <= 0) 
				{
					return TIME_END;
				}
				break;	
	
		}
		
		/* Stampo lo Score */
		draw_score(frames[SCORE], *score);

		/* Stampo le vite rimaste */
		draw_hp(frames[HP], vite, codici[UNLIMITED_HP].working);	

		/* Stampo il caricatore */
		caricatoreRana = 0;
		for (i = 0; i < NUM_FROG_BULLETS; i++) if (frog_bullets[i] == FREE_BULLET) caricatoreRana++;
		draw_frame_bullet(frames[BULLETS], caricatoreRana);
		
		if(hitbox_view)
			draw_hitboxes(frames[MAINFRAME], hit_boxes, codici[RAINBOW_FROG].working);

		/* */
		if(recoil > 0) recoil--;
		
		if(r > 0 && b == 0)
		{
			r -= 5;
			g += 5;
		}
		if(g > 0 && r == 0)
		{
			g -= 5;
			b += 5;
		}
		if(b > 0 && g == 0)
		{
			r += 5;
			b -= 5;
		}
		init_color(RGB_COLOR,r,g,b);

	}

}

_Bool are_corsie_full(_Bool corsia_libera[NUM_CORSIE])
{
	for (int i = 0; i < NUM_CORSIE; i ++)
		if(corsia_libera[i])
			return FALSE;
	return TRUE;
}

int unpack_input(Packet pkt) //:c
{
	return pkt.input;
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

Timer unpack_timer(Packet pkt) 
{
	Timer timer;

	timer.tempo_rimasto = pkt.timer.tempo_rimasto;

	return timer;
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

Bullet unpack_bullet(Packet pkt) 
{
	Bullet bullet;

	bullet.pid = pkt.bullet.pid;
	bullet.pos.r = pkt.bullet.pos.r;
	bullet.pos.c = pkt.bullet.pos.c;
	bullet.speed = pkt.bullet.speed;
	bullet.dir = pkt.bullet.dir;
	bullet.starting = pkt.bullet.starting;

	return bullet;
}

Exit_status frog_collision(Frog frog, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], Cheat_Code codici[NUM_CHEATS])
{
	int count_water = 0;
	
	//Pulizia collisioni precedenti

	if(frog.prev.r >= 0 && frog.prev.c >= 0)
		for (int r = frog.prev.r; r < frog.prev.r + FROG_H; r++)
			for (int c = frog.prev.c; c < frog.prev.c + FROG_W; c++)	
				hit_boxes[r][c] = SAFE;	
		
	for (int r = frog.pos.r; r < frog.pos.r + FROG_H; r++)
	{
		for (int c = frog.pos.c; c < frog.pos.c + FROG_W; c++)
		{
			switch(hit_boxes[r][c])
			{
				case HB_CAR:
					return SQUEEZED;

				case HB_BULLET:
					if (!codici[NO_BULLET_COLLISION].working) 
						return SHOT;

				case HB_ENEMY:
				case HB_BORDER:
					return SHOT;

				case HB_TANA:
					tiles[frog.pos.r / CELL_H][frog.pos.c / CELL_W] = X;
					return FILLED;
					
				case HB_WATER:
					hit_boxes[r][c] = HB_FROG;
					count_water++;
					if(count_water == CELL_W * CELL_H)
						return DROWNED;

				case SAFE:
					hit_boxes[r][c] = HB_FROG;		
			}
		}
	}
				
	return NOT_EXIT;
}

_Bool car_collision(Car car, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W])
{
	if(car.h_dir == LEFT && car.pos.c + car.width < MAINFRAME_W)
	{
		//Pulisci le hitbox dietro
		hit_boxes[car.pos.r][car.pos.c + car.width] = SAFE;
		hit_boxes[car.pos.r + 1][car.pos.c + car.width] = SAFE;
	}
	if(car.h_dir == RIGHT && car.pos.c - 1 >= 0)
	{
		//Pulisci le hitbox dietro
		hit_boxes[car.pos.r][car.pos.c - 1] = SAFE;
		hit_boxes[car.pos.r + 1][car.pos.c - 1] = SAFE;
	}
	
	for (int r = car.pos.r; r < car.pos.r + CELL_H; r++)
	{
		for (int c = max(0, car.pos.c); c < min(MAINFRAME_W, car.pos.c + car.width); c++)
		{
			if(hit_boxes[r][c] == HB_FROG)
				return TRUE;
			else
				hit_boxes[r][c] = HB_CAR;
		}
	}
		
	return FALSE;		
}

_Bool log_collision(Log log, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W])
{
	
	for (int i = 0; i < MAINFRAME_W; i ++)
	{
		if(hit_boxes[log.pos.r][i] == HB_FROG)
		{
			if(i >= log.pos.c && i < log.pos.c + log.length)
			{
				hit_boxes[log.pos.r][i] = SAFE;
				hit_boxes[log.pos.r + 1][i] = SAFE;
			}
			else
			{
				hit_boxes[log.pos.r][i] = HB_WATER;
				hit_boxes[log.pos.r + 1][i] = HB_WATER;
			}
		}
		else if(i >= log.pos.c && i < log.pos.c + log.length)
		{
			if(log.has_enemy)
			{
				if(hit_boxes[log.pos.r][i] == HB_FROG)
					return TRUE;
				else
				{
					hit_boxes[log.pos.r][i] = HB_ENEMY;
					hit_boxes[log.pos.r + 1][i] = HB_ENEMY;
				}
			}
			else
			{
				if(hit_boxes[log.pos.r][i] == HB_FROG)
				{
					//NON FARE NULLA, SALTA AL PROSSIMO
				}
				else
				{
					hit_boxes[log.pos.r][i] = SAFE;
					hit_boxes[log.pos.r + 1][i] = SAFE;
				}
				
			}
		}
		else
		{
			hit_boxes[log.pos.r][i] = HB_WATER;
			hit_boxes[log.pos.r + 1][i] = HB_WATER;
		}
	}
		
	return FALSE;		
}

/* La funzione restituisce la corsia intalcaso abbia colliso con essa */
Bullet_Collision_Status bullet_collision(Bullet bullet, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], Tiles tiles[GAME_H][GAME_W], _Bool sparato[NUM_LOGS])
{
	
	/* Prelevo la posizione del proiettile */
	int r = bullet.pos.r;
	int c = bullet.pos.c;

	/* Se il proiettile della rana esce dal bordo superiore */
	if (r < 0) {

		/* Rimuovo la hitbox */
		hit_boxes[0][c] = HB_BORDER;
		return UPPER_BORDER;

	} else if (r >= MAINFRAME_H) { /* Se il proiettile nemico esce dal bordo inferiore */

		/* Rimuovo la hitbox */
		hit_boxes[MAINFRAME_H - 1][c] = SAFE;
		return BOTTOM_BORDER;

	} else { /* Controllo che il proiettile non abbia colpito un oggetto */

		switch (hit_boxes[r][c]) {

			case HB_FROG:
				if (bullet.dir == DOWN) return HIT_FROG;
				break;
			case HB_ENEMY:
				if (bullet.dir == UP) {

				switch (r - 1) 
				{
					case CORRENTEFIUME1_R:
						sparato[CORRENTE1] = TRUE;
						break;
					case CORRENTEFIUME2_R:
						sparato[CORRENTE2] = TRUE;
						break;
					case CORRENTEFIUME3_R:
						sparato[CORRENTE3] = TRUE;
						break;
					case CORRENTEFIUME4_R:
						sparato[CORRENTE4] = TRUE;
						break;
					case CORRENTEFIUME5_R:
						sparato[CORRENTE5] = TRUE;
						break;	
				}

					return HIT_ENEMY; 
				}
				else return NO_HIT;
				break;
			case HB_BULLET:

				/* Entrambi i proiettili vengono cancellati dalla matrice di collisione */
				switch(tiles[r / CELL_H][c / CELL_W]) {
					case T:
						hit_boxes[r - bullet.dir][c] = HB_TANA;
						break;
					case W:
						hit_boxes[r - bullet.dir][c] = HB_WATER;
						break;
					case X:
						hit_boxes[r - bullet.dir][c] = HB_BORDER;
						break;
					default:
						hit_boxes[r - bullet.dir][c] = SAFE;
						break;
				}
				return BULLET_CLASH; 

				break;
			case HB_CAR:
				return HIT_CAR;
				break;
			default:
				hit_boxes[r][c] = HB_BULLET;
				break;

		}

		/* Pulisco la matrice di collisione nella posizione precedente del proiettile */
		if (hit_boxes[r - bullet.dir][c] != HB_FROG && hit_boxes[r - bullet.dir][c] != HB_ENEMY) {

			switch(tiles[(r - bullet.dir) / CELL_H][c / CELL_W])
			{
				case T:
					hit_boxes[r - bullet.dir][c] = HB_TANA;
					break;
				case W:
					hit_boxes[r - bullet.dir][c] = HB_WATER;
					break;
				case X:
					hit_boxes[r - bullet.dir][c] = HB_BORDER;
					break;
				default:
					hit_boxes[r - bullet.dir][c] = SAFE;
					break;
			}

		}

	}

	return NO_HIT;
}


/* Instradamento dei pacchetti verso la pipe corretta */
void log_router(int fd[NUM_PIPES][2], int spawn, Log log)
{
	switch(log.pos.r)
	{
		case CORRENTEFIUME1_R:
			write(fd[TO_LOG_1][WRITE_END], &spawn, sizeof(_Bool));
			break;
		case CORRENTEFIUME2_R:
			write(fd[TO_LOG_2][WRITE_END], &spawn, sizeof(_Bool));
			break;
		case CORRENTEFIUME3_R:
			write(fd[TO_LOG_3][WRITE_END], &spawn, sizeof(_Bool));
			break;
		case CORRENTEFIUME4_R:
			write(fd[TO_LOG_4][WRITE_END], &spawn, sizeof(_Bool));
			break;
		case CORRENTEFIUME5_R:
			write(fd[TO_LOG_5][WRITE_END], &spawn, sizeof(_Bool));
			break;
	}
}

Difficolta scegli_difficolta() {

	/* Sezione Dichiarativa */
	int max_r, max_c, riga, colonna, cursore;

	/* Ottengo le dimensioni massime dello schermo */
	getmaxyx(stdscr, max_r, max_c);

	/* Calcolo il centro dello schermo per la finestra */
	riga = (max_r - MENU_H)/2 + 1;
	colonna = (max_c - MENU_W)/2;

	/* Creo la finestra di selezione difficolta' */
	WINDOW* win = newwin(MENU_H,MENU_W,riga,colonna);

	/* Imposto il colore della finestra */
	wbkgd(win, COLOR_PAIR(0)); 

	/* Mostro il contorno della finestra */
	box(win,ACS_VLINE,ACS_HLINE);

	/* Aggiorno */
	wrefresh(win);

	/* Stampo la freccia nella posizione iniziale */
	cursore = 0;
	mvwprintw(win,3+cursore,2,"➤");
	
	/* Ciclo di selezione difficolta */
	while (true) {

		/* Mostro l'output */
		mvwprintw(win, 1, 5, "SELEZIONA DIFFICOLTA' ");
		mvwprintw(win, 3, 4, "Facile ");
		mvwprintw(win, 4, 4, "Normale ");
		mvwprintw(win, 5, 4, "Difficile ");
		mvwprintw(win, 6, 4, "Impossibile ");

		/* Abilito l'utilizzo di tasti "speciali", come le freccette */
		keypad(win, TRUE);

		/* Gestisco l'input dell'utente */
		switch (wgetch(win)) {
			case KEY_DOWN:
			case 's':
			case 'S':
				cursore = (cursore + 1) % MAX_DIFF;
				break;
			case KEY_UP:
			case 'w':
			case 'W':
				if (cursore == MIN_DIFF) cursore = MAX_DIFF-1;
				else cursore--;
				break;
			case INVIO:
				wclear(win);
				wrefresh(win);
				delwin(win);
				return (Difficolta)cursore;
				break;
		}

		/* Cancello i vecchi cursori */
		mvwprintw(win,3,2," ");  
		mvwprintw(win,4,2," ");
		mvwprintw(win,5,2," ");
		mvwprintw(win,6,2," ");
		mvwprintw(win,7,2," ");

		/* Stampo il nuovo cursore */
		mvwprintw(win, 3+cursore, 2, "➤");

		/* Aggiorno lo schermo */
		wrefresh(win);
	}
	delwin(win);
}

int getFirstFreeSlot(pid_t caricatore[], int num_proiettili) {

	for (int cont = 0; cont < num_proiettili; cont++) 
	{

		if (caricatore[cont] == FREE_BULLET) return cont;	

	}

	return -1; //Nessuno slot libero

}

int researchBullet(pid_t caricatore[], int num_proiettili, pid_t wanted) {

	for (int cont = 0; cont < num_proiettili; cont++) 
	{

		if (caricatore[cont] == wanted) return cont;

	}

	return -1; //Proiettile non trovato

}
