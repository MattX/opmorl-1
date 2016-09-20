/*
 * player.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#include <stdlib.h>
#include "opmorl.h"

int val_pos(int x, int y)
{
	if(x < 0 || x > 10 || y < 0 || y > 9 ||
			lvl_map[x][y] == T_WALL || lvl_map[x][y] == T_NONE)
		return 0;
	return 1;
}

/** find_walls : find edges (walls) of the room.
 * @param w_up upper wall
 * @param w_down lower wall
 * @param w_left left wall
 * @param w_right right wall
 * @note this function is a bit buggy.
 */

void find_walls(int * w_up, int * w_down, int * w_left, int * w_right)
{
	int cx = rodney.posx, cy = rodney.posy;

	//Find left & right walls
	while(lvl_map[cx][--cy] != T_WALL);
	*w_left = cy;
	while(lvl_map[cx][++cy] != T_WALL);
	*w_right = cy;
	cy = rodney.posy;

	//Find up & down walls
	while(lvl_map[--cx][cy] != T_WALL);
	*w_up = cx;
	while(lvl_map[++cx][cy] != T_WALL);
	*w_down = cx;
}


void check_visit()
{
	int i, j;
	int w_up, w_down, w_left, w_right;

	//Backup seen areas
	for(i = 0; i < 12; i++)
		for(j = 0; j < 11; j++)
			if(map_status[i][j] == TS_SEEN) map_status[i][j] = TS_DARK;

	//If in a corridor
	if(lvl_map[rodney.posx][rodney.posy] == T_CORRIDOR) {
		for(i = rodney.posx-1; i <= rodney.posx+1; i++)
			for(j = rodney.posy-1; j <= rodney.posy+1; j++)
				map_status[i][j] = TS_SEEN;
		return;
	}

	//If in a room
	if(lvl_map[rodney.posx][rodney.posy] == T_FLOOR ||
			lvl_map[rodney.posx][rodney.posy] == T_STAIRS) {
		find_walls(&w_up, &w_down, &w_left, &w_right);
		for(i = w_up; i <= w_down; i++)
			for(j = w_left; j <= w_right; j++)
				map_status[i][j] = TS_SEEN;
	}
}

void move_letter(char c)
{
	switch(c) {
	case 'h': // move <-
		if(val_pos(rodney.posx, rodney.posy-1) == 0) return;
		rodney.posy--;
		break;
	case 'j': // move v
		if(val_pos(rodney.posx+1, rodney.posy) == 0) return;
		rodney.posx++;
		break;
	case 'k': // move ^
		if(val_pos(rodney.posx-1, rodney.posy) == 0) return;
		rodney.posx--;
		break;
	case 'l': // move ->
		if(val_pos(rodney.posx, rodney.posy+1) == 0) return;
		rodney.posy++;
		break;
	}
	check_visit();
}
