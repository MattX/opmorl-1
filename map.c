/*
 * map.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

void corridor_gen();
void big_gen();


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


/** fill_map - generate a map
 * So, we can generate two kinds of maps : a big room or
 * two small rooms. There is a 50% chance to get a big room.
 */


void fill_map()
{
	if(lvl_nb % 7 == 0) {
		make_town();
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

/* This code is trivial, no comment. */ /*Then why a comment 1 line after ? LOL */
/* NOTE : Here we do _not_ reset Rodney's position. It's not
 * a bug, it's a feature. And I don't see how it could be any kinda bug.
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


/* Totally new function here. (added sunday jan. 10th : Well, new...)
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
	int x_stairs, y_stairs, pos; // these will be used to define the new stairs position, ROLFMAO ! OMG I'M LISTENING TO WEIRD MUSIC (the bed, berlin, lou reed)
	
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
		while (rodney.posx == (u1 = rnd_max(7, 13)));	//We want to be sure
														//Rodney won't die 
														//crushed by walls.
	else
		u1 = 21;
	if(rooms_down == 2) {
		while (rodney.posx == (d1 = rnd_max(7, 13)));		//Same
		d2 = 21;
	} else {
		while (rodney.posx == (d1 = rnd_max(5, 7)));		//Same
		while (rodney.posx == (d2 = rnd_max(d1+4, 13)));	//And same
	}
	
	//Position of stairs, cuz they rock too much not to be random
	// Upper or lower ?
	pos = rnd_max(0, 1);
	
	//First, the ordinate, as we know 0, 4, 5, 6 and 11 are walls/corridor 
	//(doesn't depend on rooms)
	if (!pos)
		x_stairs = rnd_max(1, 3);
	else
		x_stairs = rnd_max(7, 10);
	//Then, we shall determinate the abscissa 
	//basing on the number and position of rooms.
	if (!pos) { // Up !
		if (rooms_up) //Middle !
			y_stairs = rnd_max(1, 20);
		else {
				if (!rnd_max(0, 1)) //Left !
					y_stairs = rnd_max(1, u1-1);
				else //Right !
					y_stairs = rnd_max(u1+1, 20);
		}
	}

	else { //Down !
		if (rooms_down == 2) {
			if (!rnd_max(0, 1))	//Left !
				y_stairs = rnd_max(1, d1-1);
			else				//Right !
				y_stairs = rnd_max(d1+1, 20);
		}
		else {
			pos = rnd_max(0, 2); // Three choices here. Right, middle, or left ?
			if (!pos)	//Left !
				y_stairs = rnd_max(1, d1-1);
			else if (!(pos-1)) // Middle !
				y_stairs = rnd_max(d1+1, d2-1);
			else	//Right !
				y_stairs = rnd_max(d2+1, 20);
		}
	}
	//Done !
	lvl_map[x_stairs][y_stairs] = T_STAIRS;
	
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
		lvl_map[5][i] = T_CORRIDOR; // Oh yes, and we build the corridor 
									//at the same time.
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
	
	//We don't need to move rodney's pos, it is the former stairs one.
	//Except if the former stairs pos is in a wall in the next level.
	if(lvl_map[rodney.posx][rodney.posy] == T_WALL ||
			lvl_map[rodney.posx][rodney.posy] == T_NONE)
		rodney.posx = rodney.posy = 2;
}
