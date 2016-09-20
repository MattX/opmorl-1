/*
 * ai.c
 *
 *  Created on: 30 janv. 2010
 *      Author: matthieu
 */

#include "opmorl.h"

//Yes, i know the following is dirty.
static int was_mod = 0;
static int lvl_stat[12][22];
static int ox, oy;


int is_go_valid(int x, int y) {
	if(x < 0 || x > 11 || y < 0 || y > 21) return 0;
	if(lvl_map[x][y] != T_FLOOR && lvl_map[x][y] != T_CORRIDOR &&
			lvl_map[x][y] != T_STAIRS) return 0;
	if(get_monster(x, y)) return 0;
	return 1;
}

int is_rod_valid(int x, int y) {
	if(!is_go_valid(x, y)) return 0;
	if(x == rodney.posx && y == rodney.posy) return 0;
	return 1;
}

void replace(int x, int y, int clvl)
{
	if(!is_go_valid(x, y) && !(x == ox && y == oy)) return;
	if(lvl_stat[x][y] < clvl) return;
	lvl_stat[x][y] = clvl;
	was_mod = 1;
}

void find_min(int x, int y) {
	Monster * mon = get_monster(x, y);

	if(is_rod_valid(x - 1, y) && lvl_stat[x - 1][y] < lvl_stat[x][y])
		mon->posx--;
	else if(is_rod_valid(x + 1, y) && lvl_stat[x + 1][y] < lvl_stat[x][y])
		mon->posx++;
	else if(is_rod_valid(x, y - 1) && lvl_stat[x][y - 1] < lvl_stat[x][y])
		mon->posy--;
	else if(is_rod_valid(x, y + 1) && lvl_stat[x][y + 1] < lvl_stat[x][y])
		mon->posy++;
}

void show_stat()
{
	int i, j;

	for(i = 0; i < 12; i++) {
		for(j = 0; j < 22; j++) {
			if(lvl_map[i][j] == T_WALL)
				fprintf(stderr, "###|");
			else if(i == ox && j == oy)
				fprintf(stderr, "  X|");
			else if(get_monster(i, j))
				fprintf(stderr, "  m|");
			else
				fprintf(stderr, "%3d|", lvl_stat[i][j] % 100);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "------- EOT -------\n");
}

void mon_move(int x, int y)
{
	int cur_lvl = 0;
	int i, j;
	ox = x; oy = y; //small hack for replace()

	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++)
			lvl_stat[i][j] = 900000;
	lvl_stat[rodney.posx][rodney.posy] = 0;

	while(lvl_stat[x][y] == 900000) {
		cur_lvl++;
		for(i = 0; i < 12; i++)
			for(j = 0; j < 22; j++)
				if(lvl_stat[i][j] == cur_lvl-1) {
					replace(i-1, j, cur_lvl);
					replace(i+1, j, cur_lvl);
					replace(i, j+1, cur_lvl);
					replace(i, j-1, cur_lvl);
				}
		if(!was_mod) { //Can't access cell. May happen if monsters everywhere.
			printf("AI debug : monster @ %d, %d : can't move.\n", x, y);
			return;
		}
		was_mod = 0;
	}
#ifdef DEBUG
	show_stat();
#endif
	find_min(x, y);
}
