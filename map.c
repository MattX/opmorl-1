/*
 * map.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 *      Owner: zale
 */

#include "opmorl.h"

void corridor_gen();
void big_gen();

/** fill_map - generate a map
 * So, we can generate two kinds of maps : a big room or
 * two small rooms. There is a 50% chance to get a big room.
 * We also might want to add a third kind of map for the last level
 * (amulet level).
 */

void clear_status()
{
	int i, j;

	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++)
			map_status[i][j] = TS_UNVISITED;
}

void new_level()
{
	if(!DEBUG)
		if(lvl_map[rodney.posx][rodney.posx] != T_STAIRS)
			return;

	free_monsters(m_list);
	free_objects(o_list);

	lvl_nb++;

	clear_status();
	fill_map();

	check_visit();
}

void fill_map()
{
	if(lvl_nb % 7 == 0) {
		//make_town();
		free_monsters(m_list);
		free_objects(o_list);
		return; //There are no objects/monsters in a town.
	}
	else if(lvl_nb > 10 && lvl_nb < 16) big_gen();
	else corridor_gen();

	free_monsters(m_list);
	make_monsters();
	free_objects(o_list);
	make_objects();
}

/* This code is trivial, no comment. */
/* NOTE : Here we do _not_ reset Rodney's position. It's not
 * a bug, it's a feature.
 */
void big_gen()
{
	int i, j;

	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++) {
			if(i == 0 || j == 0 || i == 11 || j == 21)
				lvl_map[i][j] = T_WALL;
			else
				lvl_map[i][j] = T_FLOOR;
		}

	lvl_map[rnd_max(1, 10)][rnd_max(1, 20)] = T_STAIRS;
}


/* Totally new function here.
 *
 * We want : a corridor stretching to the two edges of the screen
 * at line 5, one or two rooms on the upper side and two or three
 * rooms on the lower side.
 */

void corridor_gen() {
	int rooms_up, rooms_down; // nb of rooms up, down
	int u1, d1, d2; // position of the right walls of rooms up 1, down 1, down 2.
	int dooru1, dooru2, doord1, doord2, doord3; // position of the doors of these rooms.
	int i, j;

	//Clear everything
	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++)
			lvl_map[i][j] = T_FLOOR;

	//First, make decisions.
	//Number of rooms
	rooms_up = rnd_max(1, 2);
	rooms_down = rnd_max(2, 3);

	//Position of walls
	if(rooms_up == 2)
		u1 = rnd_max(7, 13);
	else
		u1 = 21;
	if(rooms_down == 2) {
		d1 = rnd_max(7, 13);
		d2 = 21;
	} else {
		d1 = rnd_max(5, 7);
		d2 = rnd_max(d1+4, 13);
	}

	//Position of doors.
	dooru1 = rnd_max(1, u1-1);
	if(rooms_up == 2)
		dooru2 = rnd_max(u1+1, 20);

	doord1 = rnd_max(1, d1-1);
	doord2 = rnd_max(d1+1, d2-1);
	if(rooms_down == 3)
		doord3 = rnd_max(d2+1, 20);

	//Now, draw the border walls : horizontally
	for(i = 0; i < 22; i++) {
		lvl_map[0][i] = lvl_map[11][i] = lvl_map[4][i] = lvl_map[6][i] = T_WALL;
		lvl_map[5][i] = T_CORRIDOR; // Oh yes, and we build the corridor at the same time.
	}
	// vertically.
	for(i = 0; i < 12; i++) {
		if(i == 5) continue; //On corridor line
		lvl_map[i][0] = lvl_map[i][21] = T_WALL;
	}

	//Ok, separating walls now. For top part :
	if(rooms_up == 2)
		for(i = 0; i < 5; i++)
			lvl_map[i][u1] = T_WALL;
	//and bottom part.
	for(i = 6; i < 11; i++) {
		lvl_map[i][d1] = T_WALL;
		if(rooms_down == 3)
			lvl_map[i][d2] = T_WALL;
	}

	//Draw doors.
	lvl_map[4][dooru1] = T_CORRIDOR;
	if(rooms_up == 2)
		lvl_map[4][dooru2] = T_CORRIDOR;
	lvl_map[6][doord1] = lvl_map[6][doord2] = T_CORRIDOR;
	if(rooms_down == 3)
		lvl_map[6][doord3] = T_CORRIDOR;

	//TODO: Modify player & stairs position.
	lvl_map[9][20] = T_STAIRS;
	rodney.posx = rodney.posy = 2;
}
