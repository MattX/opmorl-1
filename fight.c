/*
 *  fight.c
 *  OPMORL0
 *
 *  Created by Théotime Grohens on 13/01/10.
 *  Copyright 2010 Opmorl, inc. All rights reserved.
 *
 *	Purpose : handle all fight funcs in one file, for evident reasons.
 *
 */

#ifndef FIGHT_H_
#define FIGHT_H_

#include "opmorl.h"
#include "objet.h"

/* ABSOLUTELY TO BE CALLED WHEN A MONSTER MOVES ONTO RODNEY'S POSITION	*/
int m_fight()
{
	Monster * mon;
	char val;
	
	if((mon = get_monster(rodney.posx, rodney.posy)) != NULL) {
		rodney.hp -= mon->attack;
		printf("The %s hits you for %d damage.\n", mon->name, mon->attack);
		fflush(stdout);
	}
	if (rodney.hp < 1) {
		if(DEBUG) {
			printf("Die(y/n) ? ");
			getchar();
			val = getchar();
			if(tolower(val) == 'n') {
				rodney.hp = 10;
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

//TODO: fix in & around here the exp, lvl and maxhp code.
//TODO: merge bow() with this function, at least partially.
int p_fight(int x, int y)
{
	Monster * mon = get_monster(x, y);
	int base_att = (rodney.exp_lvl/2) + (rodney.sword_b/20);
	
	if(mon == NULL) return 2;
	if (weapon->class == C_SWORD) {
		mon->hp -= base_att + weapon->attack;
		rodney.sword_b++;
	}
	//bow code here.
	else
		mon->hp -= base_att + 1;

	rodney.exp_b++;

	mon->awake = 1;
	if(mon->hp <= 0) {
		rm_monster(x, y);
		rodney.exp_b++;
		return 1;
	}

	check_exp_lvl();

	return 0;
}


#endif /*FIGHT_H_*/
