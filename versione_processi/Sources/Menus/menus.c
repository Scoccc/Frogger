/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "menus.h"

void sizes_menu() 
{
	int max_r, max_c;
	char c = INVIO;
	
	getmaxyx(stdscr,max_r,max_c);
	
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
		
	while(max_r < MIN_R || max_c < MIN_C || c != INVIO)
	{
		clear();
		getmaxyx(stdscr,max_r,max_c);
		
		attrset(0); //mette i colori di default
		mvprintw(0,(max_c - 11)/2,"ATTENZIONE!\n");
		printw("Le dimensioni del tuo terminale non vanno bene.\n");
		printw("Prova ad aumentarle finchè il numero di righe e colonne diventeranno verdi, il colore rosso indica che sono sotto la soglia minima consentita. Una volta aggiustate cerca di non cambiare dimensione del terminale fino alla fine del programma.\n\n");
		printw("Righe: ");
		
		if(max_r < MIN_R) 
			attron(COLOR_PAIR(1));
		else
			attron(COLOR_PAIR(2));
		printw("%d\n",max_r);
		
		attrset(0);
		printw("Colonne: ");
		
		
		
		if(max_c < MIN_C) 
			attron(COLOR_PAIR(1));
		else
			attron(COLOR_PAIR(2));
		printw("%d\n\n",max_c);
		
		if(max_r >= MIN_R && max_c >= MIN_C)
			printw("PREMI INVIO PER CONTINUARE");
			
		c = getch(); //e' bloccante, perche' ogni ridimensionamento e' un input
		
		refresh();
	}
	
	clear();
	refresh();
}

void title_menu() 
{
	int r=1000,g=0,b=0,riga,colonna, max_r, max_c;
	
	getmaxyx(stdscr,max_r,max_c);
	
	riga = (max_r - TITLE_H)/2 + 1;
	colonna = (max_c - TITLE_W)/2;
	
	WINDOW* win = newwin(TITLE_H,TITLE_W,riga,colonna);
	do
	{
		init_color(RGB_COLOR,r,g,b);
		init_pair(1,RGB_COLOR,COLOR_BLACK);
		
		wattron(win, COLOR_PAIR(1));
	    	
		mvwprintw(win,0,0,"███████╗██████╗  ██████╗  ██████╗  ██████╗ ███████╗██████╗ \n");
		mvwprintw(win,1,0,"██╔════╝██╔══██╗██╔═══██╗██╔════╝ ██╔════╝ ██╔════╝██╔══██╗\n");
		mvwprintw(win,2,0,"█████╗  ██████╔╝██║   ██║██║  ███╗██║  ███╗█████╗  ██████╔╝\n");
		mvwprintw(win,3,0,"██╔══╝  ██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  ██╔══██╗\n");
		mvwprintw(win,4,0,"██║     ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗██║  ██║\n");
		mvwprintw(win,5,0,"╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
		mvwprintw(win,6,0," by Michele Cocco & Matteo Aresu\n");
		wattrset(win, 0);
		mvwprintw(win,8,0,"                  [PRESS ENTER TO CONTINUE]                  ");
		
		wrefresh(win);
		usleep(2000);	
		
		if(r>0&&b==0)
		{
			r--;
			g++;
		}
		if(g>0&&r==0)
		{
			g--;
			b++;
		}
		if(b>0&&g==0)
		{
			r++;
			b--;
		}
	    	
		wtimeout(win,1); //aspetta un millisecondo e vai avanti
	}
	while(wgetch(win) != INVIO);
	beep();
	
	wclear(win);
	wrefresh(win);
	delwin(win);
}

/* La scelta dell'utente viene restituita tramite le apposite return.
   L'enumerazione "Menu" ha questo scopo. */
Menu game_menu() 
{
	/* Sezione Dichiarativa */
	int riga, colonna, max_r, max_c;
	Menu select = NEW_GAME;
	
	/* Sezione Esecutiva */
	/* Ottengo le dimensioni dello schermo */
	getmaxyx(stdscr,max_r,max_c);
	riga = (max_r - MENU_H)/2 + 1;
	colonna = (max_c - MENU_W)/2;
	
	/* Creo la finestra del menu' */
	WINDOW* win = newwin(MENU_H,MENU_W,riga,colonna);
	wbkgd(win, COLOR_PAIR(0)); //mette un colore di bck alla finestra
	box(win,ACS_VLINE,ACS_HLINE);
	
	/* Stampo le opzioni del menu' */
	mvwprintw(win,1,6,"SCEGLI UN' OPZIONE     ");
	mvwprintw(win,3,4,"Nuova partita          ");
	mvwprintw(win,4,4,"Visualizza classifica  ");
	mvwprintw(win,5,4,"Impostazioni           ");
	mvwprintw(win,6,4,"Menu' segreto          ");
	mvwprintw(win,7,4,"Esci                   ");
	
	/* Stampo la freccia nella posizione iniziale */
	mvwprintw(win,3,2,"➤");
	
	/* Ciclo di selezione opzione */
	while(true)
	{
		/* Imposto lo sfondo di colore nero */
		wbkgd(win, COLOR_PAIR(0));

		/* Mostro il contorno della finestra */
		box(win,ACS_VLINE,ACS_HLINE);

		/* Abilito l'utilizzo di tasti "speciali", come le freccette */
		keypad(win, TRUE);
		
		/* 
		Gestisco l'input dell'utente tramite tasto premuto.
		Il menu' funziona a rotazione ciclica, implementata tramite il modulo.
		Se si sale sopra al primo, si torna all'ultimo e viceversa.
		Ogni volta che si preme un tasto, il terminale emette un suono tramite la funzione beep.
		*/
		switch(wgetch(win))
		{
			/* Freccia verso il basso oppure 's' */
			case KEY_DOWN:
			case 's':
			case 'S':
				select = (select + 1) % MAX_MENU; 
				beep();
				break;
			
			/* Freccia verso l'alto oppure 'w' */
			case KEY_UP:
			case 'w':
			case 'W':
				if(select == 0)
					select = MAX_MENU-1;
				else
					select--;
				beep();
				break;

			/* Tasto Invio, in questo caso e' necessario cancellare tutte le finestre ed uscire dal menu' */
			case INVIO:
				beep();
				wclear(win);
				wrefresh(win);
				delwin(win);
				return select;
		}
		
		/* Cancello i vecchi ➤ */
		mvwprintw(win,3,2," ");  
		mvwprintw(win,4,2," ");
		mvwprintw(win,5,2," ");
		mvwprintw(win,6,2," ");
		mvwprintw(win,7,2," ");
		
		/* Stampo la nuova posizione di ➤ */
		mvwprintw(win,3+select,2,"➤");
		wrefresh(win);
	}

	/* Uscita dal menu' */
	wclear(win);
	delwin(win);
}

void secret_menu() 
{
	/* Sezione Dichiarativa */
	int riga, colonna, max_r, max_c, cont = 0;
	bool esci = false;
	char codice[30], buff[BUFF_SIZE]; 
	FILE *opzioni, *tmp, *scores;
	Cheat_Code codici[NUM_CHEATS];

	/* Sezione Esecutiva */
	/* In questa schermata e' possibile vedere il proprio output */
	echo();
	curs_set(1);

	/* Apro options.ini per i cheatcodes */
	opzioni = fopen ("Main/options.ini", "r");
	if (opzioni == NULL)
	{
		perror("Errore apertura file opzioni.\n");
		exit(ERR_SECRET_MENU_LOADING);
	}

	/* Mi reco alla label corretta */
	fgoto(opzioni, "[Cheats_Code]");

	/* Leggo tutti i cheats code */
	while (fscanf(opzioni, "%s : %d", codici[cont].code, &codici[cont].working) == 2 && cont < NUM_CHEATS) {
		cont++;
	}

	/* Ottengo le dimensioni dello schermo */
	getmaxyx(stdscr, max_r, max_c);
	riga = (max_r - SECRET_MENU_H) / 2 + 1;
	colonna = (max_c - SECRET_MENU_W) / 2;

	/* Creo la finestra del menu' segreto */
	WINDOW* win = newwin(SECRET_MENU_H, SECRET_MENU_W, riga, colonna);
	box(win, ACS_VLINE, ACS_HLINE);
	
	/* Stampo le informazioni del menu' segreto */
	mvwprintw(win,1,6,"INSERISCI IL TUO CODICE");
	mvwprintw(win,3,2,"➤");
	mvwprintw(win,5,6,"(Scrivi 'exit' per uscire)"); 

	/* Ciclo di selezione */
	while (!esci) {

		/* Ottengo l'input utente */
		for (int i = 4; i < SECRET_MENU_W - 1; i++) mvwprintw(win, 3, i, " ");
		mvwscanw(win,3,4,"%s", codice);

		/* Pulisco la riga output del cheat code */
		for (int i = 6; i < SECRET_MENU_W - 1; i++) mvwprintw(win, 7, i, " ");

		/* Tramuto tutta la stringa in maiuscolo */
		noCaseSensitive(codice);

		/* Controllo il codice inserito */
		if (strcmp(codice,"EXIT") == 0) {
			esci = true;

		} else if (strcmp(codice,"SALVATOREMARIOCARTA") == 0) {
			codici[UNLIMITED_HP].working = !codici[UNLIMITED_HP].working;
			mvwprintw(win, 7, 6, "%s %d", codici[UNLIMITED_HP].code, codici[UNLIMITED_HP].working);

		} else if (strcmp(codice,"LIVIOPOMPIANU") == 0) {
			codici[UNLIMITED_TIME].working = !codici[UNLIMITED_TIME].working;
			mvwprintw(win, 7, 6, "%s %d", codici[UNLIMITED_TIME].code, codici[UNLIMITED_TIME].working);

		} else if (strcmp(codice,"ANDREALODDO") == 0) {
			codici[NO_CAR_COLLISION].working = !codici[NO_CAR_COLLISION].working;
			mvwprintw(win, 7, 6, "%s %d", codici[NO_CAR_COLLISION].code, codici[NO_CAR_COLLISION].working);

		} else if (strcmp(codice,"RICCARDOSCATENI") == 0) {
			codici[NO_BULLET_COLLISION].working = !codici[NO_BULLET_COLLISION].working;
			mvwprintw(win, 7, 6, "%s %d", codici[NO_BULLET_COLLISION].code, codici[NO_BULLET_COLLISION].working);

		} else if (strcmp(codice,"MATTEOARESU") == 0) {
			codici[RAINBOW_FROG].working = !codici[RAINBOW_FROG].working;
			mvwprintw(win, 7, 6, "%s %d", codici[RAINBOW_FROG].code, codici[RAINBOW_FROG].working);

		} else if (strcmp(codice,"MICHELECOCCO") == 0) {
			codici[ALTERNATIVE_GRAPHICS].working = !codici[ALTERNATIVE_GRAPHICS].working;
			mvwprintw(win, 7, 6, "%s %d", codici[ALTERNATIVE_GRAPHICS].code, codici[ALTERNATIVE_GRAPHICS].working);

		} else if (strcmp(codice,"DELETESCORE") == 0) {
			scores = fopen("Main/scores.txt", "w");
			fclose(scores);
			mvwprintw(win, 7, 6, "punteggi cancellati");

		} else if (strcmp(codice,"DEFAULT") == 0) {

			/* Disattivo tutti i cheat code */
			for (cont = 0; cont < NUM_CHEATS; cont++) codici[cont].working = 0;
			mvwprintw(win, 7, 6, "trucchi disattivati");
		}

		/* Se il codice inserito e' errato */
		else mvwprintw(win,5,6,"Il codice inserito non e' valido.");
		
		/* Aggiorno la finestra */
		wrefresh(win);

	}

	/* Salvo gli eventuali cambiamenti */
	/* Creo un file temporaneo chiamato 'tmp.ini' */
	tmp = fopen("Main/tmp.ini", "w");
	if (tmp == NULL)
	{
		perror("Errore apertura file temporaneo.\n");
		exit(ERR_SECRET_MENU_LOADING);
	}

	/* Copio tutto da options.ini fino alla label dei cheat code */
	rewind(opzioni);
	fgoto(opzioni, "[Cheats_Code]");
	fprintf(tmp, "[Cheats_Code]\n");
	
	/* Copio i nuovi cheat code */
	for (cont = 0; cont < NUM_CHEATS; cont++) {
		fprintf(tmp, "%s : %d\n", codici[cont].code, codici[cont].working);
		fscanf(opzioni, "%s : %d", codici[cont].code, &codici[cont].working); //mando avanti anche l'altro file
	}

	/* Salto la riga vuota */
	fgets(buff, BUFF_SIZE, opzioni);	

	/* Continua la copia da file */
	while(fgets(buff, BUFF_SIZE, opzioni) != NULL)
		fputs(buff, tmp);

	/* Chiudo i file */
	fclose(opzioni);
	fclose(tmp);

	/* tmp.ini diventa il nuovo options.ini */
	remove("Main/options.ini");
	rename("Main/tmp.ini", "Main/options.ini");

	/* Elimino la finestra */
	wclear(win);
	wrefresh(win);
	delwin(win);

	/* Disattivo l'output */
	noecho();
	curs_set(0);

	/* Ritorno al menu' principale */
	return;

}

void color_change_menu()
{
	
	/* Sezione Dichiarativa */
	/* TIPO			NOME					DESCRIZIONE */
	WINDOW 			*win,
					*info;
	int 			r, 
					g, 
					b, 
					riga, 
					id, 					//
					curs = 0, 
					current_line, cur_color = 0, max_r, max_c;
	Colors clrs[3];
	char buff[BUFF_SIZE];
	char name[BUFF_SIZE];
	_Bool discar = true, quit = false;
	FILE* tmp;
	
	/* Sezione Esecutiva */
	/* */
	getmaxyx(stdscr,max_r, max_c);
	win = newwin(COLOR_MENU_H, COLOR_MENU_W, (max_r - COLOR_MENU_H) / 2, (max_c - COLOR_MENU_W) / 2);
	box(win, ACS_VLINE, ACS_HLINE);
	
	/* Apertura file opions.ini */
	FILE* opzioni = fopen("Main/options.ini", "r");
	if (opzioni == NULL) 
	{
		perror("Errore apertura file opzioni.\n");
		exit(ERR_MENU_COLORS_FAILED);
	}

	/* Imposto il colore font nero e lo sfondo bianco */
	init_pair(BW_PAIR, COLOR_BLACK, COLOR_WHITE);
	
	/* Abilita i tasti speciali, tra cui le freccette */
	keypad(win, TRUE);
	
	/* Ciclo di Esecuzione Impostazioni */
	while(!quit)
	{		
		/* Imposto la riga a 1 per non intersecare la stampa con il contorno della finestra */
		riga = 1;

		box(win, ACS_VLINE, ACS_HLINE);

		/*  Stampo in alto alla finestra la scritta [Colors].
			riga++ viene utilizzato per mandare a capo dopo aver stampato una riga. 
		*/
		mvwprintw(win,riga++,1,"[Colors]");

		/* Stampo in fondo alla finestra l'indicazione per vedere i comandi */
		mvwprintw(win,COLOR_MENU_H - 2,4,"Premere 'i' per vedere i comandi");

		/* Torno all'inizio del file */
		rewind(opzioni);

		/* Vado alla label [Colors]
		   Vedere dichiarazione funzione su utilities.h
		 */ 
		fgoto(opzioni, "[Colors]");
		
		/* Lettura da file options.ini
		   Continuo a leggere finche' esiste questa formattazione. */
		while(fscanf(opzioni, "%s %d [%d,%d,%d]", name, &id, &r, &g, &b) == 5)
		{	
			
			/* Se il cursore e' sopra il colore selezionato */
			if(id == curs + FROG_COLOR) //frog_color e' il primo pair modificabile
			{	

				/* Se l'utente vuole salvare */
				if(discar)
				{
					clrs[R] = r;
					clrs[G] = g;
					clrs[B] = b;
					discar = false;
				}
				
				/* */
				init_color(MENU_MIXED_COLOR, clrs[R], clrs[G], clrs[B]);
				init_pair(MENU_PAIR, MENU_MIXED_COLOR, MENU_MIXED_COLOR);
				wattron(win, COLOR_PAIR(MENU_PAIR));

				/* Stampo un quadrato colorato per l'anteprima dei colori */
				mvwprintw(win, riga + 1, 21, "      ");
				mvwprintw(win, riga + 2, 21, "      ");
				mvwprintw(win, riga + 3, 21, "      ");
				wattrset(win, 0);
				
				/* Se ti trovi nell'ultimo colore */
				if(id - FROG_COLOR == NUM_MENU_PAIRS - 1)
				{
					mvwprintw(win, riga, 2, "┗╸");
					wattron(win, COLOR_PAIR(BW_PAIR));
					mvwprintw(win, riga++, 4, " %s ", name);
					wattrset(win, 0);
					mvwprintw(win, riga++, 2, "   ┣╸%cred%c:   %4d", cur_color == R ? '[' : ' ', cur_color == R ? ']' : ' ', clrs[R]);
					mvwprintw(win, riga++, 2, "   ┣╸%cgreen%c: %4d", cur_color == G ? '[' : ' ', cur_color == G ? ']' : ' ', clrs[G]);
					mvwprintw(win, riga++, 2, "   ┗╸%cblue%c:  %4d", cur_color == B ? '[' : ' ', cur_color == B ? ']' : ' ', clrs[B]);
				}
				else
				{
					mvwprintw(win, riga, 2, "┣╸");
					wattron(win, COLOR_PAIR(BW_PAIR));
					mvwprintw(win, riga++, 4, " %s ", name);
					wattrset(win, 0);
					mvwprintw(win, riga++, 2, "┃  ┣╸%cred%c:   %4d", cur_color == R ? '[' : ' ', cur_color == R ? ']' : ' ',clrs[R]);
					mvwprintw(win, riga++, 2, "┃  ┣╸%cgreen%c: %4d", cur_color == G ? '[' : ' ', cur_color == G ? ']' : ' ',clrs[G]);
					mvwprintw(win, riga++, 2, "┃  ┗╸%cblue%c:  %4d", cur_color == B ? '[' : ' ', cur_color == B ? ']' : ' ',clrs[B]);
					mvwprintw(win, riga++, 2, "┃");
				}
			}
			else /* Stampa tutti gli altri colori non selezionati (senza espandere l'albero) */
			{
				if(id - FROG_COLOR == NUM_MENU_PAIRS - 1)
					mvwprintw(win, riga++, 2, "┗╸%s", name);
				else
					mvwprintw(win, riga++, 2, "┣╸%s", name);		
			}

		}
		wrefresh(win);
		
		/* Gestione pressione dei tasti */
		switch(wgetch(win))
		{
			/* Mi sposto tra i colori e tra le categorie */
			case 'w':
			case KEY_UP:
				
				if(cur_color == 0)		
				{
					if(curs == 0)
						curs = NUM_MENU_PAIRS - 1; 
					else
						curs--;
					cur_color = 2;
					
					discar = true;
				}
				else
					cur_color--;

				wclear(win);
				break;

			/* Shift + w, mi sposto solo tra le categorie */
			case 'W':				

				if(curs == 0)
					curs = NUM_MENU_PAIRS - 1;
				else
					curs--;
				
				discar = true;

				wclear(win);
				break;
			case 's':
			case KEY_DOWN:
				if(cur_color == 2)		
				{
					curs = (curs + 1) % NUM_MENU_PAIRS;
					cur_color = 0;
					discar = true;
				}
				else
					cur_color++;
					
				wclear(win);
				break;
			case 'S':

				curs = (curs + 1) % NUM_MENU_PAIRS;
				discar = true;
				
				wclear(win);
				break;
			case '-': 
			case 'a':
			case KEY_LEFT:
				clrs[cur_color] = max(0, clrs[cur_color] - 1);
				break;
			case '+':
			case 'd':
			case KEY_RIGHT:
				clrs[cur_color] = min(1000, clrs[cur_color] + 1);
				break;
			case '_': 
			case 'A':
				clrs[cur_color] = max(0, clrs[cur_color] - 10);
				break;
			case '*': 
			case 'D':
				clrs[cur_color] = min(1000, clrs[cur_color] + 10);
				break;
			case 'r': 
				clrs[R] = 0;
				break;
			case 'R': 
				clrs[R] = 1000;
				break;
			case 'g': 
				clrs[G] = 0;
				break;
			case 'G': 
				clrs[G] = 1000;
				break;
			case 'b': 
				clrs[B] = 0;
				break;
			case 'B': 
				clrs[B] = 1000;
				break;
			case INVIO: //Salva
				tmp = fopen("Main/tmp.ini", "w");
				current_line = 0;
				
				if (tmp == NULL) 
				{
					perror("Errore apertura file temporaneo.\n");
					exit(ERR_MENU_COLORS_FAILED);
				}
				
				rewind(opzioni);
				
				while(strncmp(buff, "[Colors]", strlen("[Colors]")) != 0)
				{
					fgets(buff, BUFF_SIZE, opzioni);
					fputs(buff, tmp);
				}
				
				current_line = 0;
				while(fscanf(opzioni, "%s %d [%d,%d,%d]", buff, &id, &r, &g, &b) == 5)
				{
					if(current_line == curs)
						fprintf(tmp, "%-20s\t%2d [%d,%d,%d]\n", buff, id, clrs[R], clrs[G], clrs[B]);
					else
						fprintf(tmp, "%-20s\t%2d [%d,%d,%d]\n", buff, id, r, g, b);
						
					current_line++;
				}
				
				while(fgets(buff, BUFF_SIZE, opzioni) != NULL)
					fputs(buff, tmp);
			
				fclose(tmp);
				fclose(opzioni);
				
				remove("Main/options.ini");
				rename("Main/tmp.ini", "Main/options.ini");
				
				FILE* opzioni = fopen("Main/options.ini", "r");
				if (opzioni == NULL) 
				{
					perror("Errore apertura file opzioni.\n");
					exit(-1);
				}
				
				mvwprintw(win, curs + 4, 28, "[SAVED]");
				
				break;
			case 'q':
			case 'Q':
				quit = true;
				break;
			case 'i':
			case 'I':
				
				info = newwin(INFO_H, INFO_W, (max_r - INFO_H) / 2, (max_c - INFO_W) / 2);
				box(info, ACS_VLINE, ACS_HLINE);
				wbkgd(info, COLOR_PAIR(BW_PAIR));
				mvwprintw(info, 1, 2,"     COME CAMBIARE I COLORI");
				mvwprintw(info, 3, 2,"[∆ / W]: spostati su");
				mvwprintw(info, 4, 2,"[SHIFT W]: sezione precedente");
				mvwprintw(info, 5, 2,"[∇ / S]: spostati giù");
				mvwprintw(info, 6, 2,"[SHIFT S]: sezione successiva");
				mvwprintw(info, 7, 2,"[⊳ / D]: aumenta di poco il valore");
				mvwprintw(info, 8, 2,"[⊲ / A]: diminuisci di poco il valore");
				mvwprintw(info, 9, 2,"[SHIFT D]: aumenta il valore");
				mvwprintw(info,10, 2,"[SHIFT A]: diminuisci il valore");
				mvwprintw(info,11, 2,"[R]: rosso = 0");
				mvwprintw(info,12, 2,"[G]: verde = 0");
				mvwprintw(info,13, 2,"[B]: blu = 0");
				mvwprintw(info,14, 2,"[SHIFT R]: rosso = 1000");
				mvwprintw(info,15, 2,"[SHIFT G]: verde = 1000");
				mvwprintw(info,16, 2,"[SHIFT B]: blu = 1000");
				mvwprintw(info,17, 2,"[INVIO]: salva i valori correnti");
				mvwprintw(info,18, 2,"[Q]: Esci (non salva i valori)");
				mvwprintw(info,19, 2,"Premere un qualsiasi tasto per");
				mvwprintw(info,21, 2,"chiudere questa finestra");
				wrefresh(info);
				
				wgetch(info);
				wclear(info);
				wrefresh(info);
				delwin(info);
				
				break;
		}
	}
	
	fclose(opzioni);
	wclear(win);
	wrefresh(win);
	delwin(win);		
}

void game_over(WINDOW* win)
{
	int riga = (MAINFRAME_H - 11) / 2, colonna = (MAINFRAME_W - 34) / 2;
	wattron(win, COLOR_PAIR(BW_PAIR));
	_Bool exit = false;
	_Bool m[11][35] = 
	{
		{0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1},
		{1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,0,0},
		{1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,0,0},
		{1,1,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,0,0,0,0},
		{0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0},
		{0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0},
		{0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0},
		{0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,0},
		{0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,1,0}
	};
	int a = 0;
	do
	{
		for(int i = 0; i < 11; i++)
			for (int j = 0; j < 35; j++)
				if(m[i][j])
					mvwprintw(win,riga + i, colonna + j, "%s", a%2 ? "█" : "▒");    
					
		mvwprintw(win,riga + 12, (MAINFRAME_W - 21) / 2, "[PRESS ENTER TO EXIT]");  
		wrefresh(win);
		
		a++;
		
		wtimeout(win, 500);
		if(wgetch(win) == INVIO)	
			exit = true;
	}
	while(!exit);
	
}

void vittoria(WINDOW* win)
{
	int riga = (MAINFRAME_H - 11) / 2, colonna = (MAINFRAME_W - 34) / 2;
	wattron(win, COLOR_PAIR(BW_PAIR));
	_Bool exit = false;
	_Bool m[11][28] = 
	{
		{0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,0},
		{0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0},
		{0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,1},
		{1,1,0,0,0,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,0,0,0,1,1},
		{1,1,0,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0,1,1},
		{1,1,0,1,1,1,0,1,1,0,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,1,1},
		{0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1,1}
	};
	int a = 0;
	do
	{
		for(int i = 0; i < 11; i++)
			for (int j = 0; j < 28; j++)
				if(m[i][j])
					mvwprintw(win,riga + i, colonna + j, "%s", a%2 ? "█" : "▒");    
					
		mvwprintw(win,riga + 12, (MAINFRAME_W - 21) / 2, "[PRESS ENTER TO EXIT]");  
		wrefresh(win);
		
		a++;
		
		wtimeout(win, 500);
		if(wgetch(win) == INVIO)	
			exit = true;
	}
	while(!exit);
}

void view_scores_menu()
{
	FILE* scores = fopen("Main/scores.txt", "r");
	WINDOW* win;
	int max_r, max_c, riga = 1, score, diff;
	char name[BUFF_SIZE];
	char* diffToStr[] = {"Facile", "Normale", "Difficile", "Impossibile"};
	if(scores == NULL)
	{
		perror("Errore apertura file scores.txt\n");
		exit(VIEW_SCORES_FAILED);
	}
	
	getmaxyx(stdscr,max_r, max_c);
	win = newwin(VIEW_SCORE_MENU_H, VIEW_SCORE_MENU_W, (max_r - VIEW_SCORE_MENU_H) / 2, (max_c - VIEW_SCORE_MENU_W) / 2);
	box(win, ACS_VLINE, ACS_HLINE);
	mvwprintw(win, 0, 28, "┬");
	mvwprintw(win, 0, 36, "┬");
	mvwprintw(win, riga++, 1, " %-25s │ %s │ %s", "NOME", "SCORE", "DIFFICOLTÀ");
	mvwprintw(win, riga++, 0, "├───────────────────────────┼───────┼─────────────┤");
	while(fscanf(scores, "%s %d %d", name, &score, &diff) == 3 && riga < COLOR_MENU_H - 3)
	{
		mvwprintw(win, riga++, 1, " %-25s │ %5d │ %s", name, score, diffToStr[diff]);
	}
	while(riga < COLOR_MENU_H - 3)
	{
		mvwprintw(win, riga, 28, "│");
		mvwprintw(win, riga++, 36, "│");
	}
	mvwprintw(win, riga++, 0, "├───────────────────────────┴───────┴─────────────┤");
	mvwprintw(win, riga++,(VIEW_SCORE_MENU_W - 21) / 2, "Press any key to exit");
	fclose(scores);
	
	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);
}

void insertNewScore_menu(int score, int diff)
{
	FILE* scores, *temp;
	char name[MAX_NAME_SIZE], n[MAX_NAME_SIZE];
	WINDOW* win;
	int max_r, max_c, riga = 1, s, d;
	_Bool scritto = FALSE;
	float moltiplicatoreScore[] = {0.5, 1, 1.5, 2};
	
	echo();
	getmaxyx(stdscr,max_r, max_c);
		
	scores = fopen("Main/scores.txt", "r");
	temp = fopen("Main/temp.txt", "w");

	if(scores == NULL || temp == NULL)
	{
		perror("Errore apertura file scores.txt o di temp.txt\n");
		exit(INSERT_SCORES_FAILED);
	}
	
	win = newwin(INSERT_SCORE_H, INSERT_SCORE_W, (max_r - INSERT_SCORE_H) / 2, (max_c - INSERT_SCORE_W) / 2);
	box(win, ACS_VLINE, ACS_HLINE);
	
	mvwprintw(win, riga++,1, "     IL TUO PUNTEGGIO     ");
	mvwprintw(win, riga++,1, "                          ");
	mvwprintw(win, riga++,1, "   %6d Score", score);
	mvwprintw(win, riga++,1, " * %6.1f Moltiplicatore",moltiplicatoreScore[diff]);
	score *= moltiplicatoreScore[diff];
	mvwprintw(win, riga++,1, " = %6d Score finale", score);
	
	
	wrefresh(win);
	wgetch(win);
	
	
	wclear(win);
	box(win, ACS_VLINE, ACS_HLINE);
	curs_set(1);
	
	riga = 1;
	mvwprintw(win, riga++,1, "Scrivi il tuo nome; oppure");
	mvwprintw(win, riga++,1, "  non scrivere nulla per  ");
	mvwprintw(win, riga++,1, " non salvare il punteggio ");
	mvwprintw(win, riga++,0, "├──────────────────────────┤");
	
	wmove(win, riga, 1);
	wscanw(win, "%s", name);
	
	if(strcmp("", name)!= 0)
	{
		while(fscanf(scores, "%s %d %d", n, &s, &d) == 3)
		{
			if(score > s && !scritto)
			{
				fprintf(temp, "%-25s %5d %d\n", name, score, diff);
				scritto = TRUE;
			}
			
			fprintf(temp, "%-25s %d %d\n", n, s, d);
		}
		
		if(!scritto)
			fprintf(temp, "%-25s %d %d\n", name, score, diff);
			
		remove("Main/scores.txt");
		rename("Main/temp.txt", "Main/scores.txt");
	}
					
	noecho();
	curs_set(0);
	fclose(scores);
	fclose(temp);
	
	wclear(win);
	wrefresh(win);
	delwin(win);
	
}
