/*
 * init.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

Monster yendor =
	{ 0, 0, "The mighty Wizard of Yendor", 40, 1, 0, 0, 1, 25, 200, NULL, 1000 };
int yendor_wands_wounds = 0;
int yendor_potions_healing = 5;

/* first_init - init vars
 *
 */

void first_init()
{
	int i;

	rodney.gold = 300;
	rodney.posx = 1;
	rodney.posy = 1;
	rodney.sword_b = 0;
	rodney.bow_b = 0;
	rodney.exp_lvl = 1;
	rodney.hp = rodney.max_hp = 10;
	rodney.arrows = 0;

	regain_hp_in = 5;

	weapon = malloc(sizeof(Object));
	*weapon = o_default[7];

	armor = NULL;
	shield = NULL;
	for (i = 0; i < 10; i++)
		inventory[i] = NULL;

	m_list = NULL;

	lvl_nb = 1;
	turn = 0;
	turn_spent = 0;

	clear_status();
}
