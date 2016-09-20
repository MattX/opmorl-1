/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "objet.h"
#include "opmorl.h"

/*** VERY IMPORTANT NOTE : FUNCTION NAMES CHANGED TO USE
 *** THE SAME SPEC AS IN MONSTER.C. PLEASE CHECK OUT OBJET.H
 ***/

Object add_object(Object obj) {
	Object *new = malloc(sizeof(Object));

	*new = obj;
	strcpy(new->name, obj.name);
	return *new;
}

int isSomething(int posx, int posy) {
	if (lvl_map[posx][posy] == T_FLOOR || lvl_map[posx][posy] == T_CORRIDOR)
		if (isObject(posx, posy) || isMonster(posx, posy))
			return 1;
		return 0;
	return 1;
}

void rm_object(int posx, int posy)
{
	Object * current = obj_list;
	Object * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		free(current);
		obj_list = current->next;
	}
	while(current->next != NULL) {
		if(current->next->posx == posx && current->next->posy == posy) {
			tmp = current->next->next;
			free(current->next);
			current->next = tmp;
			break;
		}
		current = current->next;
	}
}

int find_near_free_tile(int * posx, int * posy) {
	int ok = 0;
	int curx = rodney.posx;
	int cury = rodney.posy;
	if (isSomething(curx-1, cury) == 0) {
		*posx = curx-1;
		ok++;
	}
	else if (isSomething(curx, cury+1) == 0) {
		*posy = cury+1;
		ok++;
	}
	else if (isSomething(curx+1, cury) == 0) {
		*posx = curx+1;
		ok++;
	}
	else if (isSomething(curx, cury-1) == 0) {
		*posy = cury-1;
		ok++;
	}
	else if (isSomething(curx-1, cury-1) == 0) {
		*posx = curx-1;
		*posy = cury-1;
		ok++;
	}
	else if (isSomething(curx-1, cury+1) == 0) {
		*posx = curx-1;
		*posy = cury+1;
		ok++;
	}
	else if (isSomething(curx+1, cury+1) == 0) {
		*posx = curx+1;
		*posy = cury+1;
		ok++;
	}
	else if (isSomething(curx+1, cury-1) == 0) {
		*posx = curx+1;
		*posy = cury-1;
		ok++;
	}

	if (ok)
		return 1;
	return 0;
}

/* Sauf que isMonster == get_monster et isObject == get_object */

int isObject(int posx, int posy) {
	//TODO
	return 0;
}

int isMonster(int posx, int posy) {
	//TODO
	return 0;
}