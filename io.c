/*
 * io.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale (shame on him)
 *	Copyright : Opmorl, inc.
 */

#include <stdio.h>
#include "opmorl.h"

void display_msg(char * str)
{
	printf("%s\n", str);
}

void display_map()
{
	int i, j;
	for(i = 0; i < 12; i++) {
		for(j = 0; j < 22; j++) {
			if(i == rodney.posx && j == rodney.posy) {
				putchar('@');
				continue;
			}

			if(map_status[i][j] == TS_UNVISITED) {
				putchar(' ');
				continue;
			}
			
			if(get_monster(i, j) != NULL && map_status[i][j] == TS_SEEN) {
				putchar(get_monster(i, j)->name[0]);
				continue;
			}

			if(get_object(i, j) != NULL && map_status[i][j] == TS_SEEN) { 
					//We print objects before, so that if there is both
					//a monster & an object the monster is displayed.
					//Edit : this fails actually, I don't wanna imagine if there are both, it should bug.
					//Bug confirmed (and removed) : it prints the object.
				switch (get_object(i, j)->class) {
					case C_ARMOR_B:
					case C_ARMOR_S:
					case C_ARROW:
						putchar('a');
						break;
					case C_BOW:
						putchar('b');
						break;
					case C_POTION:
						putchar('p');
						break;
					case C_SWORD:
						putchar('s');
						break;
					case C_WAND:
						putchar('w');
						break;
					case C_GOLD:
						putchar('$');
						break;
				}
						 
				continue;
			}

			switch(lvl_map[i][j]) {
			case T_FLOOR:
				putchar('.');
				break;
			case T_WALL:
				putchar('#');
				break;
			case T_CORRIDOR:
				putchar('=');
				break;
			case T_NONE:
				putchar(' ');
				break;
			case T_STAIRS:
				putchar('>');
				break;
			case T_DOOR:
				putchar('+');
				break;
			}
		}
		putchar('\n');
	}
}
