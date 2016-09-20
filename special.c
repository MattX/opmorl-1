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
 * (c) the graveyard (lvl 18 to 20) //Maybe you could have told me about that... //TODO: decide wether to do this.
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


/* RETURN VALUES FOR SELL & BUY
 * 0 = quit shop()
 * 1 = next round is sell()
 * 2 = next round is buy().
 */

/* WARNING : New shop stuff is untested. */

int sell()
{
	int object, i;
	Object * obj;

	printf("You have : \n");
	for (i = 0; i < 10; i++)
			printf("%d. A %s. Value : %d\n", i, inventory[i]->name, (int)(0.66*(double)inventory[i]->val)); //We sell objects at 2/3 of their value. OK
	if (weapon)
		printf("Equipped weapon (10) : a %s, value %d\n", weapon->name, (int)(0.66*(double)weapon->val));
	if (armor)
		printf("Equipped armor  (11) : a %s, value %d\n", armor->name, (int)(0.66*(double)armor->val));
	if (shield)
		printf("Equipped shield (12) : a %s, value %d\n", shield->name, (int)(0.66*(double)shield->val));

	printf("What do you want to sell (number/B/Q) ? ");
	scanf("%c", &object);

	if(object == 'B')
		return 2;
	else if(object == 'Q')
		return 0;

	object -= 48;

	if(object < 0 || !inventory[object] || (object == 10 && !weapon)||
				(object == 11 && !armor) || (object == 12 && !shield)) {
		printf("You cannot sell this.\n");
		return 1;
	} else {
		obj = amgo(object);
		rodney.gold += 0.66*(double)obj->val;
		free(obj);
		just_dropped(object);
	}

	return 1;
}

int buy()
{
//TODO: buy.
	printf("BUY\n");
	return 1;
}

void shop()
{
	int retval = 1;

	printf("Welcome to the shop ! Here, you can buy and sell objects !\n\n");

	while(retval) {
		printf("You have %d gold.\n", rodney.gold);
		switch(retval) {
		case 1:
			retval = sell();
		case 2:
			retval = buy();
		}
	}
	
	printf("\"Bye ! Hope to see ya soon !!\", says the happy shopkeeper as you leave.\n");
}
