/*
 *  save.c
 *  OPMORL
 *
 *  Created by Théotime Grohens on 31/01/10.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */


/* This is a temporary saving system. Loading will be (or not) implemented *
 * in the future; also, it has no sense from an iPhone's POV ; therefore,  *
 * this file is for demonstration only, even if it works (and it does, RLY)*/

#include <stdio.h>
#include <stdarg.h>
#include "opmorl.h"
#include "objet.h"

#define bckp(...) fprintf(savefile, __VA_ARGS__) //The overpowerful macro

//May be called by clean_exit

void save() {
	int i, j;
	FILE* savefile = fopen("save.opm", "w+");
	Object *tmp = o_list;
	Monster *tmp2 = m_list;
	bckp("Life : %d, max %d\n", rodney.hp, rodney.max_hp);
	bckp("Level : %d\n", lvl_nb);
	bckp("Gold : %d\n", rodney.gold);
	bckp("Arrows : %d\n", rodney.arrows);
	bckp("Bonuses : bow %d, sword %d, XP level %d, XP %d\n" ,rodney.bow_b, rodney.sword_b, rodney.exp_lvl, rodney.exp_b);
	bckp("Position : %d %d\n", rodney.posx, rodney.posy);
	bckp("Seed : %d\n", seed);
	if (weapon) bckp("Weapon : %s\n", weapon->name);
	if (shield) bckp("Shield : %s\n", shield->name);
	if (armor) bckp("Armor : %s\n", armor->name);
	bckp("Inventory :\n");
	for (i = 0; i < 10; i++) {
		if (inventory[i])
			bckp("A %s\n", inventory[i]->name);
	}
	bckp("SPT :\n");
	for (i = 0; i < 10; i++) {
		if (inventory[i])
			bckp("A %s\n", spt_inv[i]->name);
	}
	bckp("MAP\n\n"); 
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 22; j++) {
			switch(lvl_map[i][j]) {
				case T_FLOOR:
					bckp(".");
					break;
				case T_WALL:
					bckp("#");
					break;
				case T_CORRIDOR:
					bckp("=");
					break;
				case T_NONE:
					bckp(" ");
					break;
				case T_STAIRS:
					bckp(">");
					break;
				case T_DOOR:
					bckp("+");
					break;
			}
		}
		bckp("\n");
	}
	bckp("\n\nObjects :\n");
	while (tmp->next) {
		bckp("A %s, shots left %d, pos %d %d\n", tmp->name, tmp->shots_left, tmp->posx, tmp->posy); //EPIC BUG, I had forgotten tmp = tmp->next it made me a 400mo (true !) savefile
		tmp = tmp->next;
	}
	bckp("\nMonsters :\n");
	while (tmp2->next) {
		bckp("A %s, invisible : %d, HP : %d, pos %d %d\n", tmp2->name, tmp2->is_invisible, tmp2->hp, tmp2->posx, tmp2->posy);
		tmp2 = tmp2->next;
	}
	fclose(savefile);
}