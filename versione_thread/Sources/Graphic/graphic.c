/* * * * * * * * * * * * * * * * * * * * 
 *           FROGGER                   *
 * Autore: Michele Cocco, Matteo Aresu *
 * Matricole: 66123, 66189             *
 * * * * * * * * * * * * * * * * * * * */

/* Collegamento al file . h */
#include "graphic.h"

void draw_TIME()
{
	int max_r, max_c;
	
	getmaxyx(stdscr, max_r, max_c);
	
	attrset(0);
	mvprintw(((max_r - MAINFRAME_H) / 2) + MAINFRAME_H, ((max_c - MAINFRAME_W) / 2) + MAINFRAME_W-16, 	  "▄▄▄▄ ▄ ▄▖▗▄ ▄▄▄");
	mvprintw(((max_r - MAINFRAME_H) / 2) + MAINFRAME_H + 1, ((max_c - MAINFRAME_W) / 2) + MAINFRAME_W-16, " ▐▌  █ █▜▛█ █━━");
	mvprintw(((max_r - MAINFRAME_H) / 2) + MAINFRAME_H + 2, ((max_c - MAINFRAME_W) / 2) + MAINFRAME_W-16, " ▝▘  ▀ ▀  ▀ ▀▀▀");
	refresh();
}

void draw_tiles(WINDOW* frame, Tiles tiles[GAME_H][GAME_W])
{
	int r,c;
	for (r = 0; r < GAME_H; r += 1)
	{
		for (c = 0; c < GAME_W; c += 1)
		{
			switch(tiles[r][c])
			{
				case T:
					wattron(frame, COLOR_PAIR(TANA_PAIR));
					mvwprintw(frame, r * CELL_H, c * CELL_W, "▁▔▁▔");
					mvwprintw(frame, r * CELL_H + 1, c * CELL_W, "▁▔▁▔");
					break;
				case W:
					wattron(frame, COLOR_PAIR(WATER_PAIR));
					mvwprintw(frame, r * CELL_H, c * CELL_W, "▁▔▁▔");
					mvwprintw(frame, r * CELL_H + 1, c * CELL_W, "▁▔▁▔");
					break;
				case X:
					wattron(frame, COLOR_PAIR(BORDER_PAIR));
					mvwprintw(frame, r * CELL_H, c * CELL_W, "✜   ");
					mvwprintw(frame, r * CELL_H + 1, c * CELL_W, "  ✜ ");
					break;
				case G:
					wattron(frame, COLOR_PAIR(GRASS_PAIR));
					mvwprintw(frame, r * CELL_H, c * CELL_W, "▞▞▞▞");
					mvwprintw(frame, r * CELL_H + 1, c * CELL_W, "▞▞▞▞");
					break;
				case S:
					wattron(frame, COLOR_PAIR(STREET_PAIR));
					mvwprintw(frame, r * CELL_H, c * CELL_W, "▘▖ ▖");
					mvwprintw(frame, r * CELL_H + 1, c * CELL_W, "▝▗▘▖");
					break;
			}
		}
	}
}

void draw_frog (WINDOW* frame, Frog frog, Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], _Bool rainbow, _Bool armor)
{
	/* Sezione Dichiarativa */
	HitBoxes tmp_hb = HB_WATER;
	int count = 0;
	/* Sezione Esecutiva */
			
	switch(tiles[frog.pos.r/FROG_H][frog.pos.c/FROG_W])
	{
		case X:
		case G:
			init_pair(FROG_PAIR_1, rainbow ? RGB_COLOR : FROG_COLOR, GRASS_COLOR_1);
			init_pair(FROG_ARMOR_PAIR, FROG_ARMOR_COLOR, GRASS_COLOR_1);
			break;
		case S:
			init_pair(FROG_PAIR_1, rainbow ? RGB_COLOR : FROG_COLOR, STREET_COLOR_1);
			init_pair(FROG_ARMOR_PAIR, FROG_ARMOR_COLOR, STREET_COLOR_1);
			break;
		case W:
		case L:
			init_pair(FROG_PAIR_1, rainbow ? RGB_COLOR : FROG_COLOR, LOG_COLOR_1);
			init_pair(FROG_ARMOR_PAIR, FROG_ARMOR_COLOR, LOG_COLOR_1);
			break;
	}
	if(tiles[frog.pos.r/FROG_H][frog.pos.c/FROG_W] == W && tiles[frog.pos.r/FROG_H][(frog.pos.c + 3)/FROG_W] == W)
	{
		init_pair(FROG_PAIR_1, rainbow ? RGB_COLOR : FROG_COLOR, WATER_COLOR_2);
		init_pair(FROG_ARMOR_PAIR, FROG_ARMOR_COLOR, WATER_COLOR_2);
	}
	
	init_pair(FROG_PAIR_2, FROG_EYES_COLOR, rainbow ? RGB_COLOR : FROG_COLOR);
	
	if(armor) 
		wattron(frame, COLOR_PAIR(FROG_ARMOR_PAIR));
	else 
		wattron(frame, COLOR_PAIR(FROG_PAIR_1));

	mvwprintw(frame, frog.pos.r, frog.pos.c,"▚");
	wattron(frame, COLOR_PAIR(FROG_PAIR_2));
	mvwprintw(frame, frog.pos.r, frog.pos.c + 1,"''");

	if(armor) 
		wattron(frame, COLOR_PAIR(FROG_ARMOR_PAIR));
	else 
		wattron(frame, COLOR_PAIR(FROG_PAIR_1));

	mvwprintw(frame, frog.pos.r, frog.pos.c + 3,"▞");
	mvwprintw(frame, frog.pos.r + 1, frog.pos.c,"▟▛▜▙");
			
	if(frog.prev.r >= 0 && frog.prev.c >= 0)
	{
		switch(tiles[frog.prev.r / CELL_H][frog.prev.c / CELL_W])
		{
			case W:
			case T:
				wattron(frame, COLOR_PAIR(WATER_PAIR));
				mvwprintw(frame, frog.prev.r, frog.prev.c, "▁▔▁▔");
				mvwprintw(frame, frog.prev.r + 1, frog.prev.c, "▁▔▁▔");
				break;
			case X:
				wattron(frame, COLOR_PAIR(BORDER_PAIR));
				mvwprintw(frame, frog.prev.r, frog.prev.c, "▞▞▞▞");
				mvwprintw(frame, frog.prev.r + 1, frog.prev.c, "▞▞▞▞");
				break;
			case G:
				wattron(frame, COLOR_PAIR(GRASS_PAIR));
				mvwprintw(frame, frog.prev.r, frog.prev.c, "▞▞▞▞");
				mvwprintw(frame, frog.prev.r + 1, frog.prev.c, "▞▞▞▞");
				break;
			case S:
				wattron(frame, COLOR_PAIR(STREET_PAIR));
				mvwprintw(frame, frog.prev.r, frog.prev.c, "▘▖ ▖");
				mvwprintw(frame, frog.prev.r + 1, frog.prev.c, "▝▗▘▖");
				break;
			case L:
				wattron(frame, COLOR_PAIR(LOG_PAIR_1));
				
				for (int i = 0; i < CELL_W; i++) 
				{
					if (tmp_hb == HB_WATER && hit_boxes[frog.prev.r][frog.prev.c] == HB_WATER) 
					{
						if (frog.prev.c % 2) 
						{
							mvwprintw(frame, frog.prev.r, frog.prev.c + i, "▔");
							mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "▔");
						} 
						else 
						{
							mvwprintw(frame, frog.prev.r, frog.prev.c + i, "▁");
							mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "▁");
						}
					} 
					else if (tmp_hb == W && hit_boxes[frog.prev.r][frog.prev.c] == L) 
					{
						mvwprintw(frame, frog.prev.r, frog.prev.c + i, "▟");
						mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "▜");
					} 
					else if (tmp_hb == L && hit_boxes[frog.prev.r][frog.prev.c] == W) 
					{
						mvwprintw(frame, frog.prev.r, frog.prev.c + i - 1, "▙");
						mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i - 1, "▛");
						if (frog.prev.c % 2) 
						{
							mvwprintw(frame, frog.prev.r, frog.prev.c + i, "▔");
							mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "▔");
						} 
						else 
						{
							mvwprintw(frame, frog.prev.r, frog.prev.c + i, "▁");
							mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "▁");
						}
					} 
					else 
					{
						mvwprintw(frame, frog.prev.r, frog.prev.c + i, "█");
						mvwprintw(frame, frog.prev.r + 1, frog.prev.c + i, "█");
					}
					
					tmp_hb = hit_boxes[frog.prev.r][frog.prev.c];
				}

				break;
		}
	}
		
    	wrefresh(frame);
}

void draw_car (WINDOW* frame, Car car)
{
	int sizes[] = {CAR1_W, CAR2_W, CAR3_W, CAR4_W, CAR5_W};
	
	switch(car.car_type)
	{
		case CAR1:
			if(car.h_dir == RIGHT)
				draw_car1_dx(frame,car);
			else
				draw_car1_sx(frame,car);
			break;
		case CAR2:
			if(car.h_dir == RIGHT)
				draw_car2_dx(frame,car);
			else
				draw_car2_sx(frame,car);
			break;
		case CAR3:
			if(car.h_dir == RIGHT)
				draw_car3_dx(frame,car);
			else
				draw_car3_sx(frame,car);
			break;
		case CAR4:
			if(car.h_dir == RIGHT)
				draw_car4_dx(frame,car);
			else
				draw_car4_sx(frame,car);
			break;
		case CAR5:
			if(car.h_dir == RIGHT)
				draw_car5_dx(frame,car);
			else
				draw_car5_sx(frame,car);
			break;
	}
	
	if(car.h_dir == RIGHT)
	{
		wattron(frame, COLOR_PAIR(STREET_PAIR));
		switch(car.pos.c % 4)
		{
			case 0:
				mvwprintw(frame,car.pos.r, car.pos.c-1, "▘");
				mvwprintw(frame,car.pos.r + 1, car.pos.c-1, "▝");
				break;
			case 1:
				mvwprintw(frame,car.pos.r, car.pos.c-1, "▖");
				mvwprintw(frame,car.pos.r + 1, car.pos.c-1, "▗");	
				break;
			case 2:
				mvwprintw(frame,car.pos.r, car.pos.c-1, " ");
				mvwprintw(frame,car.pos.r + 1, car.pos.c-1, "▘");
				break;
			case 3:
				mvwprintw(frame,car.pos.r, car.pos.c-1, "▖");
				mvwprintw(frame,car.pos.r + 1, car.pos.c-1, "▖");
				break;
		}		
	}
	else
	{
		wattron(frame, COLOR_PAIR(STREET_PAIR));
		switch((car.pos.c + sizes[car.car_type])% 4)
		{
			case 0:
				mvwprintw(frame,car.pos.r, car.pos.c + sizes[car.car_type], "▘");
				mvwprintw(frame,car.pos.r + 1, car.pos.c + sizes[car.car_type], "▝");
				break;
			case 1:
				mvwprintw(frame,car.pos.r, car.pos.c + sizes[car.car_type], "▖");
				mvwprintw(frame,car.pos.r + 1, car.pos.c + sizes[car.car_type], "▗");	
				break;
			case 2:
				mvwprintw(frame,car.pos.r, car.pos.c + sizes[car.car_type], " ");
				mvwprintw(frame,car.pos.r + 1, car.pos.c + sizes[car.car_type], "▘");
				break;
			case 3:
				mvwprintw(frame,car.pos.r, car.pos.c + sizes[car.car_type], "▖");
				mvwprintw(frame,car.pos.r + 1, car.pos.c + sizes[car.car_type], "▖");
				break;
		}
	}
	
	wrefresh(frame);
}

void draw_car1_sx (WINDOW* frame, Car car)
{	

	int i;
	int pairs[2][CAR1_W] = {{CAR1_PAIR_1, CAR1_PAIR_1, CAR1_PAIR_2, CAR1_PAIR_1, CAR1_PAIR_1, CAR1_PAIR_1, CAR1_PAIR_2, CAR1_PAIR_1},
				{CAR1_PAIR_1, CAR1_PAIR_3, CAR1_PAIR_4, CAR1_PAIR_3, CAR1_PAIR_3, CAR1_PAIR_3, CAR1_PAIR_4, CAR1_PAIR_3}};
	wchar_t spritesTop[CAR1_W + 1] = L"▗▅▅▅▅▅▅▅";
	wchar_t spritesBot[CAR1_W + 1] = L"▝▂▂▂▂▂▂▂";
				
	for (i = 0; i < CAR1_W; i += 1)
	{
		if(car.pos.c + i >= 0)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}
			
    	wrefresh(frame);
}

void draw_car1_dx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR1_W] = {{CAR1_PAIR_1, CAR1_PAIR_2, CAR1_PAIR_1, CAR1_PAIR_1, CAR1_PAIR_1, CAR1_PAIR_2, CAR1_PAIR_1, CAR1_PAIR_1},
				{CAR1_PAIR_3, CAR1_PAIR_4, CAR1_PAIR_3, CAR1_PAIR_3, CAR1_PAIR_3, CAR1_PAIR_4, CAR1_PAIR_3, CAR1_PAIR_1}};				 
	wchar_t spritesTop[CAR1_W + 1] = L"▅▅▅▅▅▅▅▖";
	wchar_t spritesBot[CAR1_W + 1] = L"▂▂▂▂▂▂▂▘";
	
	for (i = 0; i < CAR1_W; i += 1)
	{
		if(car.pos.c + i < MAINFRAME_W)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}
			
    	wrefresh(frame);
}

void draw_car2_sx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR2_W] = {{CAR2_PAIR_1, CAR2_PAIR_1, CAR2_PAIR_2, CAR2_PAIR_1, CAR2_PAIR_1, CAR2_PAIR_2, CAR2_PAIR_1, CAR2_PAIR_1},
				{CAR2_PAIR_3, CAR2_PAIR_3, CAR2_PAIR_4, CAR2_PAIR_3, CAR2_PAIR_3, CAR2_PAIR_4, CAR2_PAIR_3, CAR2_PAIR_1}};				 
	wchar_t spritesTop[CAR2_W + 1] = L"▂▄▄▄▄▄▄▍";
	wchar_t spritesBot[CAR2_W + 1] = L"▆▄▄▄▄▄▄▍";
	
	for (i = 0; i < CAR2_W; i += 1)
	{
		if(car.pos.c + i >= 0)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}
			
    	wrefresh(frame);
}

void draw_car2_dx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR2_W] = {{CAR2_PAIR_1, CAR2_PAIR_1, CAR2_PAIR_2, CAR2_PAIR_1, CAR2_PAIR_1, CAR2_PAIR_2, CAR2_PAIR_1, CAR2_PAIR_1},
				{CAR2_PAIR_1, CAR2_PAIR_3, CAR2_PAIR_4, CAR2_PAIR_3, CAR2_PAIR_3, CAR2_PAIR_4, CAR2_PAIR_3, CAR2_PAIR_3}};				 
	wchar_t spritesTop[CAR2_W + 1] = L"▐▄▄▄▄▄▄▂";
	wchar_t spritesBot[CAR2_W + 1] = L"▐▄▄▄▄▄▄▆";
	
	for (i = 0; i < CAR2_W; i += 1)
	{
		if(car.pos.c + i < MAINFRAME_W)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}
			
    	wrefresh(frame);
}

void draw_car3_sx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR3_W] = {{CAR3_PAIR_2, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_2},
				{CAR3_PAIR_2, CAR3_PAIR_3, CAR3_PAIR_1, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_2}};				 
	wchar_t spritesTop[CAR3_W + 1] = L"▁▂▟▄▄▃▂▁";
	wchar_t spritesBot[CAR3_W + 1] = L"▔▆▜▄▄▅▆▔";
	
	for (i = 0; i < CAR3_W; i += 1)
	{
		if(car.pos.c + i >= 0)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    	wrefresh(frame);
}

void draw_car3_dx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR3_W] = {{CAR3_PAIR_2, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_1, CAR3_PAIR_2},
				{CAR3_PAIR_2, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_3, CAR3_PAIR_1, CAR3_PAIR_3, CAR3_PAIR_2}};				 
	wchar_t spritesTop[CAR3_W + 1] = L"▁▂▃▄▄▙▂▁";
	wchar_t spritesBot[CAR3_W + 1] = L"▔▆▅▄▄▛▆▔";
	
	for (i = 0; i < CAR3_W; i += 1)
	{
		if(car.pos.c + i < MAINFRAME_W)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    	wrefresh(frame);
}

void draw_car4_sx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR4_W] = {{CAR4_PAIR_1, CAR4_PAIR_1, CAR4_PAIR_1, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2},
				{CAR4_PAIR_1, CAR4_PAIR_3, CAR4_PAIR_3, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2}};				 
	wchar_t spritesTop[CAR4_W + 1] = L"▗▆▆▛▔▔▔▔▜";
	wchar_t spritesBot[CAR4_W + 1] = L"▝▂▂▙▁▁▁▁▟";
	
	for (i = 0; i < CAR4_W; i += 1)
	{
		if(car.pos.c + i >= 0)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    	wrefresh(frame);
}

void draw_car4_dx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR4_W] = {{CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_1, CAR4_PAIR_1, CAR4_PAIR_1},
				{CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_2, CAR4_PAIR_3, CAR4_PAIR_3, CAR4_PAIR_1}};				 
	wchar_t spritesTop[CAR4_W + 1] = L"▛▔▔▔▔▜▆▆▖";
	wchar_t spritesBot[CAR4_W + 1] = L"▙▁▁▁▁▟▂▂▘";
	
	for (i = 0; i < CAR4_W; i += 1)
	{
		if(car.pos.c + i < MAINFRAME_W)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    	wrefresh(frame);
}

void draw_car5_sx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR5_W] = {{CAR5_PAIR_1, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3},
				     {CAR5_PAIR_1, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3}};	
				     			 
	wchar_t spritesTop[CAR5_W + 1] = L"▗▛▔▔█ ┏╍╍╍╍╍╍╍╍┓ ";
	wchar_t spritesBot[CAR5_W + 1] = L"▝▙▁▁█ ┗╍╍╍╍╍╍╍╍┛ ";
	
	for (i = 0; i < CAR5_W; i += 1)
	{
		if(car.pos.c + i >= 0)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    	wrefresh(frame);
}

void draw_car5_dx (WINDOW* frame, Car car)
{
	int i;
	int pairs[2][CAR5_W] = {{CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_1},
				     {CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_3, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_2, CAR5_PAIR_1}};	
				     			 
	wchar_t spritesTop[CAR5_W + 1] = L" ┏╍╍╍╍╍╍╍╍┓ █▔▔▜▖";
	wchar_t spritesBot[CAR5_W + 1] = L" ┗╍╍╍╍╍╍╍╍┛ █▁▁▟▘";
	
	for (i = 0; i < CAR5_W; i += 1)
	{
		if(car.pos.c + i < MAINFRAME_W)
		{
			wattron(frame, COLOR_PAIR(pairs[0][i]));
    			mvwprintw(frame, car.pos.r, car.pos.c + i, "%lc", spritesTop[i]);
    			
    			wattron(frame, COLOR_PAIR(pairs[1][i]));
    			mvwprintw(frame, car.pos.r+1, car.pos.c + i, "%lc", spritesBot[i]);
		}
	}   	 				
    wrefresh(frame);
}


void draw_log(WINDOW* frame, Log log)
{
	int r = log.pos.r;
	int c = log.pos.c;
	
	wattron(frame, COLOR_PAIR(LOG_PAIR_1));
	mvwprintw(frame, r,   c, "▟");
	mvwprintw(frame, r+1, c, "▜");
	mvwprintw(frame, r,   c + log.length - 1, "▙");
	mvwprintw(frame, r+1, c + log.length - 1, "▛");
	
	if(log.has_enemy)
	{
		wattron(frame, COLOR_PAIR(ENEMY_PAIR1));
		mvwprintw(frame, r, c + 1, "▗▆▇");
		mvwprintw(frame, r+1, c+1, "▀▛▛");
		mvwprintw(frame, r,  c + log.length - 4, "▇▆▖");
		mvwprintw(frame, r+1, c + log.length - 4, "▜▜▀");
		
		for (int i = 4; i < log.length - 4; i += 1)
		{
			mvwprintw(frame, r,   c + i, "█");
			
			if((log.length % 2 == 1) && (i == log.length / 2))
				mvwprintw(frame, r+1, c + i, "▀");
			else if(i < log.length / 2)
				mvwprintw(frame, r+1, c + i, "▛");
			else
				mvwprintw(frame, r+1, c + i, "▜");
		}
	}
	else
	{
		wattron(frame, COLOR_PAIR(LOG_PAIR_1));
		for (int i = 1; i < log.length - 1; i += 1)
		{
			mvwprintw(frame, r,   c + i, "█");
			mvwprintw(frame, r+1, c + i, "█");
		}
	}
	
	wattron(frame, COLOR_PAIR(WATER_PAIR));
	if(log.h_dir == RIGHT)
	{
		for(int i = log.pos.c - 1; i >= max(0, log.pos.c - 4); i--)
		{
			if(i%2)
			{
				mvwprintw(frame,log.pos.r, i, "▔");
				mvwprintw(frame,log.pos.r + 1, i, "▔");
			}
			else
			{
				mvwprintw(frame,log.pos.r, i, "▁");
				mvwprintw(frame,log.pos.r + 1, i, "▁");
			}
		}	
	}
	else
	{	
		for(int i = log.pos.c + log.length; i <= min(MAINFRAME_W - 1, log.pos.c + log.length + 4); i++)
		{
			if(i%2)
			{
				mvwprintw(frame,log.pos.r, i, "▔");
				mvwprintw(frame,log.pos.r + 1, i, "▔");
			}
			else
			{
				mvwprintw(frame,log.pos.r, i, "▁");
				mvwprintw(frame,log.pos.r + 1, i, "▁");
			}
		}
	}
	
	wrefresh(frame);
	
}

void draw_timer(WINDOW* frame, Timer timer, _Bool timeless) 
{

	/* Sezione Dichiarativa */
	int tempo, n, i;

	if(timeless)
	{
		wattron(frame, COLOR_PAIR(RGB_PAIR)); 
		mvwprintw(frame,1,(TIMER_W - 13) / 2,"∞ UNLIMITED ∞");
		wattrset(frame, 0);
		wrefresh(frame);
		return;
	}
	
	/* Recupero il valore */
	tempo = timer.tempo_rimasto;

	/* Imposto il numero di caratteri su cui scrivere */
	n = ((TIMER_W-2) * tempo * 8)/TIME;
	for (i = 1; i < TIMER_W-1; ++i) { mvwprintw(frame,1,i," "); }

	/* Setto i colori */
	if(tempo < SOGLIA_ROSSO) init_color(TIMER_COLOR,1000,0,0); 	   		
	else if(tempo < SOGLIA_GIALLO) init_color(TIMER_COLOR,1000,1000,0);	
	else init_color(TIMER_COLOR,0,1000,0);
	init_pair(TIMER_PAIR,TIMER_COLOR,COLOR_BLACK);
	wattron(frame, COLOR_PAIR(TIMER_PAIR));      

	/* Stampo */
	for (i = 0; i < n/8; ++i) { mvwprintw(frame,1,i+1,"█"); }
	switch(n%8)
	{
		case 7:
			mvwprintw(frame,1,i+1,"▉");
			break;
		case 6:
			mvwprintw(frame,1,i+1,"▊");
			break;
		case 5:
			mvwprintw(frame,1,i+1,"▋");
			break;
		case 4:
			mvwprintw(frame,1,i+1,"▌");
			break;
		case 3:
			mvwprintw(frame,1,i+1,"▍");
			break;
		case 2:
			mvwprintw(frame,1,i+1,"▎");
			break;
		case 1:
			mvwprintw(frame,1,i+1,"▏");
			break;
	}      

	/* Aggiorno */
	wattron(frame, COLOR_PAIR(BW_PAIR)); 
	wrefresh(frame);

}

void init_colors()
{

	/* Sezione Dichiarativa */
	FILE *opzioni;
	char buff[BUFF_SIZE];
	int id, r, g, b;

	/* Sezione Esecutiva */
	/* Apertura file "options.ini" */
	opzioni = fopen("Main/options.ini", "r");
	if (opzioni == NULL) 
	{
		perror("Errore apertura file opzioni.\n");
		exit(-1);
	}
	
	/* Carico i pair di colori */
	fgoto(opzioni, "[Colors]");
			
	while(fscanf(opzioni, "%s %d [%d,%d,%d]", buff, &id, &r, &g, &b) == 5)
		init_color(id, r, g, b);
	
	init_color(BULLETS_MENU_COLOR, 1000,1000,0);
	init_color(FROG_ARMOR_COLOR, 700,700,700);
	/* Chiudo il file */
	fclose(opzioni);

	
}

void init_cp()
{
	init_pair(BW_PAIR, COLOR_WHITE, COLOR_BLACK);
	init_pair(GRASS_PAIR, GRASS_COLOR_1, GRASS_COLOR_2);
	init_pair(STREET_PAIR, STREET_COLOR_1, STREET_COLOR_2);
	init_pair(WATER_PAIR, WATER_COLOR_1, WATER_COLOR_2);
	init_pair(CAR1_PAIR_1, CAR1_COLOR, STREET_COLOR_2);
	init_pair(CAR1_PAIR_2, CAR1_COLOR, TIRE_COLOR);
	init_pair(CAR1_PAIR_3, STREET_COLOR_2, CAR1_COLOR);
	init_pair(CAR1_PAIR_4, TIRE_COLOR, CAR1_COLOR);
	init_pair(CAR2_PAIR_1, CAR2_COLOR, STREET_COLOR_2);
	init_pair(CAR2_PAIR_2, CAR2_COLOR, TIRE_COLOR);
	init_pair(CAR2_PAIR_3, STREET_COLOR_2, CAR2_COLOR);
	init_pair(CAR2_PAIR_4, TIRE_COLOR, CAR2_COLOR);
	init_pair(CAR3_PAIR_1, CAR3_COLOR, STREET_COLOR_2);
	init_pair(CAR3_PAIR_2, TIRE_COLOR, STREET_COLOR_2);
	init_pair(CAR3_PAIR_3, STREET_COLOR_2, CAR3_COLOR);
	init_pair(CAR3_PAIR_4, STREET_COLOR_2, TIRE_COLOR);
	init_pair(CAR4_PAIR_1, CAR4_COLOR, STREET_COLOR_2);
	init_pair(CAR4_PAIR_2, CAR4_COLOR, CARICO_COLOR);
	init_pair(CAR4_PAIR_3, STREET_COLOR_2, CAR4_COLOR);
	init_pair(CAR5_PAIR_1, CAR5_COLOR1, STREET_COLOR_2);
	init_pair(CAR5_PAIR_2, CAR5_COLOR1, GLASS_COLOR);
	init_pair(CAR5_PAIR_3, DETAILS_COLOR, CAR5_COLOR2);
	init_pair(CAR1_PAIR_1, CAR1_COLOR, STREET_COLOR_2);
	init_pair(CAR1_PAIR_2, CAR1_COLOR, TIRE_COLOR);
	init_pair(CAR1_PAIR_3, STREET_COLOR_2, CAR1_COLOR);
	init_pair(CAR1_PAIR_4, TIRE_COLOR, CAR1_COLOR);
	init_pair(CAR2_PAIR_1, CAR2_COLOR, STREET_COLOR_2);
	init_pair(CAR2_PAIR_2, CAR2_COLOR, TIRE_COLOR);
	init_pair(CAR2_PAIR_3, STREET_COLOR_2, CAR2_COLOR);
	init_pair(CAR2_PAIR_4, TIRE_COLOR, CAR2_COLOR);
	init_pair(CAR3_PAIR_1, CAR3_COLOR, STREET_COLOR_2);
	init_pair(CAR3_PAIR_2, TIRE_COLOR, STREET_COLOR_2);
	init_pair(CAR3_PAIR_3, STREET_COLOR_2, CAR3_COLOR);
	init_pair(CAR3_PAIR_4, STREET_COLOR_2, TIRE_COLOR);
	init_pair(CAR4_PAIR_1, CAR4_COLOR, STREET_COLOR_2);
	init_pair(CAR4_PAIR_2, CAR4_COLOR, CARICO_COLOR);
	init_pair(CAR4_PAIR_3, STREET_COLOR_2, CAR4_COLOR);
	init_pair(CAR5_PAIR_1, CAR5_COLOR1, STREET_COLOR_2);
	init_pair(CAR5_PAIR_2, CAR5_COLOR1, GLASS_COLOR);
	init_pair(CAR5_PAIR_3, DETAILS_COLOR, CAR5_COLOR2);
	init_pair(LOG_PAIR_1, LOG_COLOR_1, WATER_COLOR_2);
	init_pair(ENEMY_PAIR1, ENEMY_COLOR, LOG_COLOR_1);
	init_pair(ENEMY_PAIR2, LOG_COLOR_1, ENEMY_COLOR);
	init_pair(E_BULLET_G_PAIR, ENEMY_BULLET_COLOR, GRASS_COLOR_1);
	init_pair(F_BULLET_G_PAIR, FROG_BULLET_COLOR, GRASS_COLOR_1);
	init_pair(E_BULLET_S_PAIR, ENEMY_BULLET_COLOR, STREET_COLOR_1);
	init_pair(F_BULLET_S_PAIR, FROG_BULLET_COLOR, STREET_COLOR_1);
	init_pair(E_BULLET_W_PAIR, ENEMY_BULLET_COLOR, WATER_COLOR_2);
	init_pair(F_BULLET_W_PAIR, FROG_BULLET_COLOR, WATER_COLOR_2);
	init_pair(E_BULLET_L_PAIR, ENEMY_BULLET_COLOR, LOG_COLOR_1);
	init_pair(F_BULLET_L_PAIR, FROG_BULLET_COLOR, LOG_COLOR_1);
	init_pair(ALLERT_PAIR, COLOR_RED, STREET_COLOR_2);
	init_pair(BULLETS_MENU_PAIR, BULLETS_MENU_COLOR, COLOR_BLACK);
	init_pair(TANA_PAIR, TANE_COLOR_1, TANE_COLOR_2);
	init_pair(F_BULLET_T_PAIR, FROG_BULLET_COLOR, TANE_COLOR_2);
	init_pair(E_BULLET_T_PAIR, ENEMY_BULLET_COLOR, TANE_COLOR_2);
	init_pair(BORDER_PAIR, PLANTS_COLOR, GRASS_COLOR_1);
	init_pair(F_BULLET_X_PAIR, FROG_BULLET_COLOR, GRASS_COLOR_1);
	init_pair(E_BULLET_X_PAIR, ENEMY_BULLET_COLOR, GRASS_COLOR_1);
	init_pair(RGB_PAIR, RGB_COLOR, COLOR_BLACK);
}

void draw_score(WINDOW* frame, int score) {

	/* Stampo il punteggio */
	mvwprintw(frame, 1, 1, "Score: %6d", score);
	box(frame, ACS_VLINE, ACS_HLINE);
	wrefresh(frame);

}

void draw_hp(WINDOW* frame, int hp, _Bool immortal) 
{
	/* Stampo le vite rimaste */
	if(immortal)
	{
		wattron(frame, COLOR_PAIR(RGB_PAIR)); 
		mvwprintw(frame,1,(HP_W - 8)/ 2,"IMMORTAL");
		wattrset(frame, 0);
		wrefresh(frame);
		return;
	}
	for(int cont = 0; cont < hp; cont++) mvwprintw(frame, 1, cont*3 + 1, "🐸");
	wrefresh(frame);

}

void draw_hitboxes(WINDOW* frame, HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W], _Bool rainbow)
{
	int r,c;
	for (r = 0; r < MAINFRAME_H; r++)
	{
		for (c = 0; c < MAINFRAME_W; c++)
		{
			switch(hit_boxes[r][c])
			{
				case (HB_CAR):
					wattron(frame, COLOR_PAIR(CAR1_PAIR_1));
					break;
				case (SAFE):
					wattron(frame, COLOR_PAIR(GRASS_PAIR));
					break;
				case (HB_WATER):
					wattron(frame, COLOR_PAIR(WATER_PAIR));
					break;
				case (HB_ENEMY):
					wattron(frame, COLOR_PAIR(ENEMY_PAIR1));
					break;
				case (HB_FROG):
					wattron(frame, COLOR_PAIR(rainbow ? RGB_PAIR : FROG_PAIR_1));
					break;
				case (HB_TANA):
					wattron(frame, COLOR_PAIR(CAR3_PAIR_2));
					break;
				case (HB_BULLET):
					wattron(frame, COLOR_PAIR(E_BULLET_G_PAIR));
					break;
				case (HB_BORDER):
					wattron(frame, COLOR_PAIR(ALLERT_PAIR));
					break;
			}
			mvwprintw(frame, r, c, "█");
		}
	}
	wrefresh(frame);
}

void draw_bullet(WINDOW* frame, Bullet bullet, Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W])
{
	switch(tiles[bullet.pos.r / CELL_H][bullet.pos.c / CELL_W])
	{
		case X:
			if(bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_X_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_X_PAIR));
			break;
		case G:
			if(bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_G_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_G_PAIR));
			break;
		case S:
			if(bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_S_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_S_PAIR));
			break;
		case T:
			if (bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_T_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_T_PAIR)); 
		case W:
			if(bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_W_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_W_PAIR));
			break;
		case L:
			if(bullet.dir == UP)
				wattron(frame, COLOR_PAIR(F_BULLET_L_PAIR));
			else
				wattron(frame, COLOR_PAIR(E_BULLET_L_PAIR));
			break;
	}
	
	if(hit_boxes[bullet.pos.r][bullet.pos.c] != HB_FROG)
		mvwprintw(frame, bullet.pos.r, bullet.pos.c, "%s", bullet.dir == UP ? "•" : "⌇");
	
	if(hit_boxes[bullet.pos.r - bullet.dir][bullet.pos.c] != HB_FROG)
	{
		switch(tiles[(bullet.pos.r - bullet.dir)/ CELL_H][bullet.pos.c / CELL_W])
		{
			case X:
				wattron(frame, COLOR_PAIR(BORDER_PAIR));
				if((bullet.pos.c % CELL_W == 0 && (bullet.pos.r - bullet.dir) % CELL_H == 0) ||
				  (bullet.pos.c % CELL_W == 2 && (bullet.pos.r - bullet.dir) % CELL_H == 1))
					mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "✜");
				else
					mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, " ");
				break;
			case G:
				wattron(frame, COLOR_PAIR(GRASS_PAIR));
				mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "▞");				
				break;
			case S:
				wattron(frame, COLOR_PAIR(STREET_PAIR));
				mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "▗");
				break;
			case T:
			case W:
				wattron(frame, COLOR_PAIR(WATER_PAIR));
				if(bullet.pos.c % 2)
					mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "▔");
				else
					mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "▁");
				break;
			case L:
				wattron(frame, COLOR_PAIR(LOG_PAIR_1));
				mvwprintw(frame, bullet.pos.r - bullet.dir, bullet.pos.c, "█");
				break;	
		}
	}
	
}

void draw_frame_bullet(WINDOW *frame, int num_proiettili) {

	//Disegno il numero di proiettili
	wattron(frame, COLOR_PAIR(BULLETS_MENU_PAIR));
	for (int cont = 0; cont < NUM_FROG_BULLETS; cont++) mvwprintw(frame, 1, cont*2 + 1, " ");
	for (int cont = 0; cont < num_proiettili; cont++) mvwprintw(frame, 1, cont*2 + 1, "█");
	wrefresh(frame);
	wattron(frame, COLOR_PAIR(BW_PAIR));
}

void draw_power(WINDOW *frame, int power) {

	/* Sezione Dichiarativa */
	int n, i;

	/* Sezione Esecutiva */
	init_color(POWER_COLOR, 1000 - power * 7, 1000 - power * 7, 1000);
	init_pair(POWER_PAIR, POWER_COLOR, COLOR_BLACK);
	wattron(frame, COLOR_PAIR(POWER_PAIR));
	n = ((POWER_W-2) * power * 8) / MAX_POWER;
	for (i = 0; i < POWER_W-2; i++) mvwprintw(frame, 1, i+1, " ");
	for (i = 0; i < n/8; ++i) { mvwprintw(frame,1,i+1,"█"); }
	switch(n%8)
	{
		case 7:
			mvwprintw(frame,1,i+1,"▉");
			break;
		case 6:
			mvwprintw(frame,1,i+1,"▊");
			break;
		case 5:
			mvwprintw(frame,1,i+1,"▋");
			break;
		case 4:
			mvwprintw(frame,1,i+1,"▌");
			break;
		case 3:
			mvwprintw(frame,1,i+1,"▍");
			break;
		case 2:
			mvwprintw(frame,1,i+1,"▎");
			break;
		case 1:
			mvwprintw(frame,1,i+1,"▏");
			break;
	}      

	wrefresh(frame);
	wattron(frame, COLOR_PAIR(BW_PAIR));

}
