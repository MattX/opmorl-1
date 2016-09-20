/*
 * player.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#include <stdlib.h>
#include "opmorl.h"

#define V_NO 0
#define V_YES 1
#define V_COMBAT 2

int val_pos(int x, int y)
{
	if(x < 0 || x > 11 || y < 0 || y > 10 ||
			lvl_map[x][y] == T_WALL || lvl_map[x][y] == T_NONE)
		return V_NO;
	if(get_monster(x, y) != NULL)
		return V_COMBAT;
	return V_YES;
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
	while(val_pos(cx, --cy));
	*w_left = cy;
	while(val_pos(cx, ++cy));
	*w_right = cy;
	cy = rodney.posy;

	//Find up & down walls
	while(val_pos(--cx, cy));
	*w_up = cx;
	while(val_pos(++cx, cy));
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
			for(j = rodney.posy-1; j <= rodney.posy+1; j++) {
				map_status[i][j] = TS_SEEN;
				if(get_monster(i,j) != NULL)
					get_monster(i,j)->awake=1;
			}
		return;
	}

	//If in a room
	if(lvl_map[rodney.posx][rodney.posy] == T_FLOOR ||
			lvl_map[rodney.posx][rodney.posy] == T_STAIRS) {
		find_walls(&w_up, &w_down, &w_left, &w_right);
		for(i = w_up; i <= w_down; i++)
			for(j = w_left; j <= w_right; j++) {
				map_status[i][j] = TS_SEEN;
				if(get_monster(i,j) != NULL)
					get_monster(i,j)->awake=1;
			}
	}
}

void move_letter(char c)
{
	int ret;

	switch(c) {
	case 'h': // move <-
		ret = val_pos(rodney.posx, rodney.posy-1);

		if(!ret) return;
		if(ret == V_COMBAT) {
			//fight(rodney.posx, rodney.posy-1);
			return;
		}
		rodney.posy--;
		break;
	case 'j': // move v
		ret = val_pos(rodney.posx+1, rodney.posy);

		if(!ret) return;
		if(ret == V_COMBAT) {
			//fight(rodney.posx+1, rodney.posy);
			return;
		}
		rodney.posx++;
		break;
	case 'k': // move ^
		ret = val_pos(rodney.posx-1, rodney.posy);

		if(!ret) return;
		if(ret == V_COMBAT) {
			//fight(rodney.posx-1, rodney.posy);
			return;
		}
		rodney.posx--;
		break;
	case 'l': // move ->
		ret = val_pos(rodney.posx, rodney.posy+1);

		if(!ret) return;
		if(ret == V_COMBAT) {
		//	fight(rodney.posx, rodney.posy+1);
			return;
		}
		rodney.posy++;
		break;

	}
	check_visit();
}
