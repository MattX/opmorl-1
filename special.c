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
	"You hear a booming voice : “Welcome to the town of %s ! You will find\n"
	"a store at the north-west, a SPT station at the south-west, the castle\n" //This comment has been removed for aggraved stupidity.
	"of the Lord at the north-east and a %s at the south-east\".\n";

/* This file contains functions for :
 * (a) towns (lvls 7, 14 & 21)
 * (b) the last level & win (lvl 25) //TODO: This
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
		strcpy(name, "Enylbereth");	//Could
		strcpy(special, "School");	//not
	} else if(!lvl_nb%14) {			//you
		strcpy(name, "Angbereth");	//find
		strcpy(special, "Arena");	//shittier
	} else {						//names,
		strcpy(name, "Dolbereth");	//RLY ?
		strcpy(special, "Silicon Sapphire"); //This name rocks, and is a private joke to the Clash listeners, removing/changing is forbidden (or fork the project).
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

void shop() { //This func's gotta whole buncha loops&conditions... //O RLY ? (this comment has been added by tt's second personality
	char a = 'y', b = 'y', c;
	int i, object, j;
	printf("Welcome to the shop ! Here, you can buy and sell objects !\n\n");
		//Main loop
	while (a) {
		c = choice('b', 's', "Buy (b) or sell (s) ?");
		if (c == 's') { //Sell mode. Here, we assume we can sell our equipped items.
			while (b == 'y') { 
					//Here the guy's got objects
				if (inventory[0] || weapon || armor || shield) {
					//Inventory display
					for (i = 0; i < 10; i++) {
						if (!i) 
							printf("Inventory :\n");
						if (inventory[i]) 
							printf("%d. A %s. Value : %d\n", i, inventory[i]->name, (int)(0.66*(double)inventory[i]->val)); //We sell objects at 2/3 of their value.
					} 
					if (weapon)
						printf("Equipped weapon (10) : a %s, value %d\n", weapon->name, (int)(0.66*(double)weapon->val));
					if (armor)
						printf("Equipped armor  (11) : a %s, value %d\n", armor->name, (int)(0.66*(double)armor->val));
					if (shield)
						printf("Equipped shield (12) : a %s, value %d\n", shield->name, (int)(0.66*(double)shield->val));
					scanf("%d", &object);
					if (object < 0 ||
						!inventory[object] ||
						(object == 10 && !weapon)||
						(object == 11 && !armor) ||
						(object == 12 && !shield))
							printf("Wrong number\n");
					else {
						if (object < 10) {
							rodney.gold += 0.66*(double)inventory[object]->val;
							free(inventory[object]);
							for (i = object+1; i < 10; i++)
								inventory[i-1] = inventory[i];
							inventory[9] = NULL;
						}
						else if (object == 10) {
							rodney.gold += 0.66*(double)weapon->val;
							free(weapon);
						}
						else if (object == 11) {
							rodney.gold += 0.66*(double)armor->val;
							free(armor);
						}
						else if (object == 12) {
							rodney.gold += 0.66*(double)shield->val;
							free(shield);
						}
							
					}
				}
					//The noob wanna sell but he ain't got nothing, LØL 
				else{
					printf("You have nothing to sell.\n");
					break;
				}
				printf("You have %d gold now.\n", rodney.gold);
				
				while (b != 'y' && b != 'n'){ // Deeply fails
					printf("Sell something else (y/n) ?\n");
					b = getchar();
				}
			}
			
		}
		else { //Buy mode.
			b = 'y';
			while (b == 'y') {
				for (i = 0; i < 15; i++)
					printf("%d. A %s, price : %s\n", i, o_default[i].name, o_default[i].val);
				scanf("%d", &object);
				getchar();
				if (object < 0 || object > 15) {
					printf("Wrong object\n");
					continue;
				}
				else if (rodney.gold < o_default[object].val) {
					printf("You don't have enough gold to buy this object.\n");
					continue;
				}
				else if (inventory[9] != NULL)	 { //The inventory is full
					printf("Your inventory is full, drop or sell something.\n");
					continue;
				}
				else {
					j = 0;
					if (o_default[object].class == C_ARROW) {
						rodney.arrows += 10;
					}
					else {
						while (inventory[j++] != NULL);
						inventory[j] = *o_default[object];
						rodney.gold -= o_default[object].val;
					}
				}
				printf("You have %d gold left.\n");
				b = choice('y', 'y', "Buy something else (y/n) ?");
			}
			
		} 
		
		a = choice('y', 'n', "Leave the shop (y/n) ?\n");
	
	}
	printf("\"Bye ! Hope to see ya soon !!\", says the happy shopkeeper as you leave.\n");
}