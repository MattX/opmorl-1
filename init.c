/*
 * init.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 */

#include "opmorl.h"

/** first_init - init vars
 *
 */

void first_init()
{
	srand(time(NULL));

	rodney.gold = 0;
	rodney.posx = 1;
	rodney.posy = 1;
	rodney.hp = 10;
	rodney.arrows = 10;
	weapon = o_default[8]; 
	armor; /* If these are no pointers, how to initialize them ? */
	shield;

	m_list = NULL;

	going_up = 0;
	lvl_nb = 1;
	turn = 0;

	clear_status();
}
