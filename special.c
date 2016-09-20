/*
 * special.c
 *
 *  Created on: 17 dec. 2009
 *      Author: zale
 *		Owner : zale
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

char town_welcome[] =
	"You hear a booming voice : \"Welcome to the town of %s ! You will find\n"
	"a store at the north-west, a SPT station at the south-west, the castle\n"
	"of the Lord at the north-east and a %s at the south-east\".\n";

/* This file contains functions for :
 * (a) towns (lvls 7, 14 & 21)
 * (b) the last level & win (lvl 25) //TODO: This
 * (d) (maybe) (later) quests //We've got lots to do before quests. RLY.
 */

/* make_town (see mockups for how a town looks)
 * - fill the borders with T_WALLs.
 * - fill the building with T_WALLs, and place the doors.
 */

void make_town()
{
	char name[20];
	char special[20];
	int i, j;

	if(lvl_nb == 7) {
		strcpy(name, "Enylbereth");
		strcpy(special, "School");
	} else if(!lvl_nb%14) {
		strcpy(name, "Angbereth");
		strcpy(special, "Arena");
	} else {
		strcpy(name, "Dolbereth");
		strcpy(special, "Silicon Sapphire");
	}

	//Borders, ground & buildings 
	for(i = 0; i < 12; i++)
		for(j = 0; j < 22; j++) {
			if(j < 6 || j > 16 || i == 0 || i == 11)
				lvl_map[i][j] = T_WALL;
			else
				lvl_map[i][j] = T_FLOOR;
		}
	lvl_map[6][11] = T_STAIRS; //We couldn't escape the town...

	//Doors
	lvl_map[3][5] = lvl_map[7][5] = lvl_map[3][17] = lvl_map[7][17] = T_DOOR;

	rodney.posx = rodney.posy = 10;

	printf(town_welcome, name, special);
}



/* WARNING : New shop stuff is untested. */

void sell()
{
	int object, i;
	Object * obj;

	printf("You have : \n");
	for (i = 0; i < 10; i++)
		if (inventory[i])
			printf("%d. A %s. Value : %d\n", i, inventory[i]->name, (int)(0.66*(double)inventory[i]->val)); //We sell objects at 2/3 of their value. OK
	if (weapon)
		printf("Equipped weapon (10) : a %s, value %d\n", weapon->name, (int)(0.66*(double)weapon->val));
	if (armor)
		printf("Equipped armor  (11) : a %s, value %d\n", armor->name, (int)(0.66*(double)armor->val));
	if (shield)
		printf("Equipped shield (12) : a %s, value %d\n", shield->name, (int)(0.66*(double)shield->val));

	printf("What do you want to sell ? "); //At first sight I thought it was a word game numBer/B/Q. 
	scanf("%d", &object);

		if(object < 0 || !inventory[object] || (object == 10 && !weapon)||
				(object == 11 && !armor) || (object == 12 && !shield)) {
		printf("You cannot sell this.\n");
						} else {
		obj = amgo(object);
		rodney.gold += 0.66*(double)obj->val;
		free(obj);
		just_dropped(object);
	}
}

void buy()
{
	int i, index;
	printf("Here, you can buy : \n");
	for (i = 0; i < 16; i++) //CONSTWARN
		printf("%d. A %s. Value : %d\n", i, o_default[i].name, o_default[i].val);
	printf("What do you want to buy ?\n");
	scanf("%d", &index);
	if (o_default[index].val > rodney.gold) {
		printf("You don't have enough money, sorry.\n");
		return;
	}
	else {
		i = -1;
		rodney.gold -= o_default[index].val;
		if (o_default[index].class == C_ARROW)
			rodney.arrows += 10;
		else {
			while (inventory[++i]);
			if(i >= 10) {
				printf("Your inventory is full (of shit).\n");
				return;
			}
			inventory[i] = malloc(sizeof(Object));
			*inventory[i] = o_default[index];
		}
	}
}

void shop()
{
	int retval = 1;

	printf("Welcome to the shop ! Here, you can buy and sell objects !\n\n");

	while(retval) {
		printf("You have %d gold.\n", rodney.gold);
		printf("Do you want to buy (1), sell (2) or leave(0) ?\n");
		scanf("%d", &retval);
		if (retval == 1)
			buy();
		else if (retval)
			sell();
	}
	
	printf("\"Bye ! Hope to see ya soon !!\", says the happy shopkeeper as you leave.\n");
}

void recover() {
	int i, val, j = -1;
	if (!spt_inv[0]) {
		printf("There is nothing in the SPT.\n");
		return;
	}
	for (i = 0; spt_inv[i]; i++)
		printf("%d. A %s\n", i, spt_inv[i]->name);
	scanf("%d", &val);
	if (!spt_inv[val]) {
		printf("There is nothing here.\n");
		return;
	}
	if (inventory[9]) {
		printf("Your inventory is full.\n");
		return;
	}
	while (inventory[++j]);
	*inventory[j] = *spt_inv[val];
	free(spt_inv[val]);
	spt_inv[val] = NULL;
	for (i = val+1; i < 10; i++)
		spt_inv[i-1] = spt_inv[i]; //this should _not_ segfault
	spt_inv[9] = NULL;
}

void give_spt() {
	int i, val, j = -1;
	if (!inventory[0]) {
		printf("There is nothing in your inventory.\n");
		return;
	}
	for (i = 0; inventory[i]; i++)
		printf("%d. A %s\n", i, inventory[i]->name);
	scanf("%d", &val);
	if (!inventory[val]) {
		printf("There is nothing here.\n");
		return;
	}
	if (spt_inv[9]) {
		printf("The SPT is full.\n");
		return;
	}
	while (spt_inv[++j]);
	*spt_inv[j] = *inventory[val];
	free(inventory[val]);
	inventory[val] = NULL;
	for (i = val+1; i < 10; i++)
		inventory[i-1] = inventory[i]; //this should _not_ segfault
	inventory[9] = NULL;
}

void spt() {
	int retval, i = -1, j = -1;
	printf("Welcome to the SPT station !! Here, you can leave us some objects you will recover in the next station ! How amazing !!\n");
	while (retval) {
		while (inventory[++i]);
		while (spt_inv[++j]);
		printf("There are %d objects in your inventory, %d in the SPT.\n", i, j);
		printf("Do you want to get an object (1), give us one (2), or leave (0) ?\n");	  
		scanf("%d", &retval);
		if (retval == 1)
			recover();
		else if (retval)
			give_spt();
		i = j = -1;
	}
	printf("\"We'll take care of your stuff !\", says the SPT team as you close the door.\n");
}
