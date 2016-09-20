/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


//here's what I had done at dec. 6 hh:mm
#include <stdlib.h>
#include <string.h>
#include "objet.h"

/*** VERY IMPORTANT NOTE : FUNCTION NAMES CHANGED TO USE
 *** THE SAME SPEC AS IN MONSTER.C. PLEASE CHECK OUT OBJET.H
 ***/

Object add_object(Object obj) {
	int i;
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
	Object * current = m_list;
	Object * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		free(current);
		m_list = current->next;
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
		ok++
	}
	else if (isSomething(curx, curyy-1) == 0) {
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
