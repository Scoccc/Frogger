#include "../../Main/utilities.h"


void refresh_all (WINDOW** frames)
{
	for (int i = 0; i < NUM_FRAMES; i++)
		wrefresh(frames[i]);
}

void clear_all(WINDOW** frames)
{
	for (int i = 0; i < NUM_FRAMES; i++)
		wclear(frames[i]);
}

int max(int a, int b) { return a > b ? a : b; }

int min(int a, int b) { return a < b ? a : b; }

void fgoto(FILE* file, char* label)
{
	char buff[BUFF_SIZE];
	while(fscanf(file, "%s", buff) == 1)
		if(strcmp(buff, label) == 0)
			return;
}

void clearTiles(Tiles tiles[GAME_H][GAME_W])
{
	for (int i = 2; i <= 6; i++)
		for (int j = 0; j < GAME_W; j++)
			tiles[i][j] = W;
}

void init_hitboxes(Tiles tiles[GAME_H][GAME_W], HitBoxes hit_boxes[MAINFRAME_H][MAINFRAME_W])
{
	for (int r = 0; r < MAINFRAME_H; r++)
	{
		for (int c = 0; c < MAINFRAME_W; c++)
		{
			switch(tiles[r/CELL_H][c/CELL_W])
			{
				case G:
				case S:
					hit_boxes[r][c] = SAFE;
					break;
				case W:
					hit_boxes[r][c] = HB_WATER;
					break;
				case T:
					hit_boxes[r][c] = HB_TANA;
					break;
				case X:
					hit_boxes[r][c] = HB_BORDER;
					break;
			}
		}
	}
}

_Bool rollProbability(int tot, int prob)
{
	return rand() % tot < prob;
}

void noCaseSensitive(char *string) {

	for (int cont = 0; cont < strlen(string); cont++) {
		string[cont] = toupper(string[cont]);
	}

}
