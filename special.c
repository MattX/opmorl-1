/*
 * special.c
 *
 *  Created on: 17 dec. 2009
 *      Author: zale
 *      Owner: zale
 */

#include "opmorl.h"

char town_welcome[] =
	"You hear a booming voice : “Welcome to the town of %s ! You will find\n"
	"a store at the north-west, a SPT station at the south-west, the castle\n" //WTF is a SPT ? Sous pute transitive ? Sodomite puant transsexuel ? Stand de Production de T-shirts ?
	"of the Lord at the north-east and a %s at the south-east\".\n";

/* This file contains functions for :
 * (a) towns (lvls 7, 14 & 21)
 * (b) the last level & win (lvl 25)
 * (c) the graveyard (lvl 18 to 20) //Maybe you could have told me about that...
 * (d) (maybe) (later) quests //We gotta lot todo before quests. RLY.
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
		strcpy(name, "Enylbereth");	//You
		strcpy(special, "School");	//could
	} else if(!lvl_nb%14) {			//not
		strcpy(name, "Angbereth");	//find
		strcpy(special, "Arena");	//shittier
	} else {						//names,
		strcpy(name, "Dolbereth");	//RLY ?
		strcpy(special, "Silicon Sapphire"); //This name rocks, and is a private joke to the Clash listeners, removing/changing is forgotten (or fork the project).
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
