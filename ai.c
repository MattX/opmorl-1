/*
 * ai.c
 *
 *  Created on: 30 janv. 2010
 *      Author: matthieu
 */

#include "opmorl.h"

/* REGULAR MONSTER AI */

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


/** @param p if p == -1 find_min will become find_max
 * otherwise p should be set to 1.
 */
void find_min(int x, int y, int p) {
	Monster * mon = get_monster(x, y);

	if(is_rod_valid(x - 1, y) && p*lvl_stat[x - 1][y] < p*lvl_stat[x][y])
		mon->posx--;
	else if(is_rod_valid(x + 1, y) && p*lvl_stat[x + 1][y] < p*lvl_stat[x][y])
		mon->posx++;
	else if(is_rod_valid(x, y - 1) && p*lvl_stat[x][y - 1] < p*lvl_stat[x][y])
		mon->posy--;
	else if(is_rod_valid(x, y + 1) && p*lvl_stat[x][y + 1] < p*lvl_stat[x][y])
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


/** @param p see find_min */
void mon_move(int x, int y, int p)
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
			fprintf(stderr, "AI debug : monster @ %d, %d : can't move.\n", x, y);
			return;
		}
		was_mod = 0;
	}
#ifdef DEBUG
	show_stat();
#endif
	find_min(x, y, 1);
}

/* YENDOR AI */
/* So, yendor wants to use his wands first. He won't do this if :
 * (a) he would shoot a monster, or
 * (b) he has no shots left, or
 * (c) he cannot shoot.
 */

void y_ai()
{
	int rod;
	fprintf(stderr, "Yendor is thinking !\n");

	fmonat(yendor.posx, yendor.posy, rodney.posx, rodney.posy, &rod);
	if(yendor.hp <= rodney.exp_lvl/2 + weapon->attack &&
			yendor_potions_healing) {
		//Rodney could kill us ! quick, a potion of healing !
		fprintf(stderr, "Yendor drank !\n");
		yendor.hp += 35;
		yendor_potions_healing--;
		return;
	}
	else if(abs(yendor.posx-rodney.posx)+abs(yendor.posy-rodney.posy) < 3 &&
			yendor_wands_wounds) {
		//RODNEY IS NEAR. FLEE !
		fprintf(stderr, "Yendor flees !\n");
		mon_move(yendor.posx, yendor.posy, -1);
		return;
	}
	else if(rod) {
		// If we shoot at rodney
		fprintf(stderr, "Yendor shoots !\n");
		rodney.hp -= 30 + (armor?armor->attack:0);
		chk_dead("the mighty wizard of Yendor");
		yendor_wands_wounds--;
		return;
	}
	else //We approach rodney
		mon_move(yendor.posx, yendor.posy, 1);
	fprintf(stderr, "Yendor approaches !\n");
}
