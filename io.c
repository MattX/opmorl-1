/*
 * io.c
 *
 *  Created on: 2 dec. 2009
 *      Author: zale
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
		for(j = 0; j < 11; j++) {
			if(i == rodney.posx && j == rodney.posy) {
				putchar('@');
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
				if(going_up) putchar ('<');
				else putchar('>');
				break;
			}
		}
		putchar('\n');
	}
}
