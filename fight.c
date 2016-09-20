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

void sing_fight(Monster * mon)
{
	if(mon == NULL) return;
/*total att = monster attack - defense : shield          - defense : armor. */
	rodney.hp -= mon->attack - (shield?shield->attack:0) - (armor?armor->attack:0);

	printf("The monster @ %d, %d hit you for %d damage.\n", mon->posx, mon->posy,
			mon->attack - (shield?shield->attack:0) - (armor?armor->attack:0));
	chk_dead("a monster");
}

int m_fight()
{
	Monster * mon;
	char val;
	
	mon = get_monster(rodney.posx-1, rodney.posy);
	sing_fight(mon);
	mon = get_monster(rodney.posx+1, rodney.posy);
	sing_fight(mon);
	mon = get_monster(rodney.posx, rodney.posy-1);
	sing_fight(mon);
	mon = get_monster(rodney.posx, rodney.posy+1);
	sing_fight(mon);
}

int p_fight(int x, int y)
{
	Monster * mon = get_monster(x, y);
	int base_att = rodney.exp_lvl/2;
	
	if(mon == NULL) return 2;
	if (weapon->class == C_SWORD) {
		mon->hp -= base_att + rodney.sword_b/20 + weapon->attack;
		rodney.sword_b++;
	} else if(weapon->class == C_BOW) {
		mon->hp -= base_att + rodney.bow_b/20 + weapon->attack;
		rodney.bow_b++;
		rodney.arrows--;
	}
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
