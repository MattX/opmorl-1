/*
 *  fight.c
 *  OPMORL
 *
 *  Created by Théotime Grohens on 13/01/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 *	Purpose : handle all fight funcs in one file, for evident reasons.
 *
 */

#ifndef FIGHT_H_
#define FIGHT_H_

#include "opmorl.h"
#include "objet.h"

/* ABSOLUTELY TO BE CALLED WHEN A MONSTER MOVES ONTO RODNEY'S POSITION,				*
 *THEREFORE TO BE CALLED FROM M_MOVE() & FIGHT() WHICH IS CALLED BY MOVE_LETTER()	*/
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

int p_fight(int x, int y)
{
	Monster * mon = get_monster(x, y);
	
	if(mon == NULL) return 2;
	printf("Weapon.class = %d\n", weapon->class); fflush(stdout);
	if (weapon->class == C_SWORD) {
		mon->hp -= weapon->attack + rodney.sword_b/50 + rodney.exp_lvl/3 + 1; 
			//OMG, you need a bonus of 50 for 1 extra damage
		rodney.sword_b++;
	}
	else if (weapon->class == C_BOW) {
		if(!rodney.arrows)
			printf("You have no arrows left.\n");
		else {
			mon->hp -= weapon->attack+  rodney.bow_b/50 + rodney.exp_lvl/3 + 1;
			rodney.bow_b++;
			rodney.arrows--;
		}
	}
	else /* Means there is no equipped weapon */
		mon->hp -= rodney.exp_lvl / 3 + 1;
	mon->awake = 1; /* Wake up monster */
	if(mon->hp <= 0) {
		rm_monster(x, y);
		return 1;
	}
	if((rodney.bow_b + rodney.sword_b) % rodney.exp_lvl*rodney.exp_lvl+10)
			//UGLY. It makes u level up at each kill.
		printf("You have ascended to level %d\n", rodney.exp_lvl++);
	return 0;
}


#endif /*FIGHT_H_*/