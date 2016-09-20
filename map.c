/*
 * map.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
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
		for(j = 0; j < 11; j++)
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
	if(rnd_max(0,1) == 1) big_gen();
	else corridor_gen();

	make_monsters();
	//make_objects();
}

/* This code is trivial, no comment. */
/* NOTE : Here we do _not_ reset Rodney's position. It's not
 * a bug, it's a feature.
 */
void big_gen()
{
	int i, j;

	for(i = 0; i < 12; i++)
		for(j = 0; j < 11; j++) {
			if(i == 0 || j == 0 || i == 11 || j == 10)
				lvl_map[i][j] = T_WALL;
			else
				lvl_map[i][j] = T_FLOOR;
		}

	lvl_map[rnd_max(1, 10)][rnd_max(1, 9)] = T_STAIRS;
}

/* OK, here we have a room of 11x4 cells on the
 * top half of the screen and a 11x5 room on the bottom half
 * of it.
 * (0) fill the map with T_NONE's
 * (a) choose the size of the two rooms
 * (b) choose the position of the doors
 * (c) draw the walls & fill the rooms
 * (d) make the corridor.
 */
void corridor_gen()
{
	int i,j;
	int room1, room2, door1, door2;

	for(i = 0; i < 12; i++)
		for(j = 0; j < 11; j++)
			lvl_map[i][j] = T_NONE;

	room1 = 4;
	room2 = 5;
	door1 = rnd_max(1,10);
	door2 = rnd_max(1,10);

	//OK, let's go
	//(c) 1- Draw the upper & lower walls
	for(i = 0; i < 11; i++)
		lvl_map[0][i] = lvl_map[11][i] = T_WALL;
	//    2- Draw the border walls
	for(i = 0; i < 12; i++) {
		if(i > room1 && i < 11-room2) continue;
		for(j = 0; j < 11; j++)
			lvl_map[i][j] = T_WALL;
	}

	//    3- fill the rooms
	for(i = 1; i < 10; i++) {
		for(j = 1; j < room1; j++)
			lvl_map[j][i] = T_FLOOR;
		for(j = 11-room2 + 1; j < 11; j++)
			lvl_map[j][i] = T_FLOOR;
	}

	//(d) Doors are type T_FLOOR actually
	lvl_map[room1][door1] = T_FLOOR;
	lvl_map[11-room2][door2] = T_FLOOR;

	for(i = min(door1, door2); i <= max(door1, door2); i++) {
		lvl_map[room1+1][i] = T_CORRIDOR;
	}

	// Generate stairs in 2nd room
	lvl_map[rnd_max(room2+2,10)][rnd_max(1,9)] = T_STAIRS;

	// Move rodney to somewhere in 1st room
	rodney.posx = rnd_max(1,room1);
	rodney.posy = rnd_max(1,9);

}
