/*
 * map.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#include "opmorl.h"

/** fill_map - generate a map
 * So, we can generate two kinds of maps : a big room or
 * two small rooms. There is a 50% chance to get a big room.
 * We also might want to add a third kind of map for the last level
 * (amulet level).
 */
//TODO: decide wether the idea is ok.

void fill_map(void)
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
