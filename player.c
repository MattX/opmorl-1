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

void move_letter(char c)
{
	switch(c) {
	case 'h': // move <-
		if(rodney.posy < 1) return;
		rodney.posy--;
		break;
	case 'j': // move v
		if(rodney.posx > 10) return;
		rodney.posx++;
		break;
	case 'k': // move ^
		if(rodney.posx < 1) return;
		rodney.posx--;
		break;
	case 'l': // move ->
		if(rodney.posy > 9) return;
		rodney.posy++;
		break;
	}
}
