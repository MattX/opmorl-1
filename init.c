/*
 * init.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

/* first_init - init vars
 *
 */

void first_init()
{
	int i;
	srand(time(NULL));

	rodney.gold = 30;
	rodney.posx = 1;
	rodney.posy = 1;
	rodney.sword_b = 0;
	rodney.bow_b = 0;
	rodney.exp_lvl = 1;
	rodney.hp = 10;
	rodney.arrows = 0;

	weapon = malloc(sizeof(Object));
	*weapon = o_default[7];

	armor = NULL;
	shield = NULL;
	for (i = 0; i < 10; i++)
		inventory[i] = NULL;

	m_list = NULL;

	lvl_nb = 1;
	turn = 0;

	clear_status();
}
