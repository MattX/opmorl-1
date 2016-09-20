/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 MyWare, inc. All rights reserved.
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
	Object * current = o_list;
	Object * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		free(current);
		o_list = current->next; 
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

/* This function is not KNOWN to work, but where could bugs hide in 5 lines of code ? */

int isObject(int posx, int posy) {
	Object *tmp = get_object(posx, posy);
	if (tmp == NULL)
		return 0;
	free(tmp);
	return 1;
	return 0;
}

/* same as up */
int isMonster(int posx, int posy) {
	Monster *tmp = get_monster(posx, posy);
	if (tmp == NULL)
		return 0;
	free(tmp);
	return 1;
}


/* Called when a new level is generated. */
void free_objects(Object * obj)
{
	if(obj == NULL) return;
	if(obj->next != NULL)
		free_objects(obj->next);
	free(obj);
}

Object * get_object(int posx, int posy)
{
	Object * current = o_list;
	
	while(current != NULL) {
		if(current->posx == posx && current->posy == posy)
			return current;
		current = current->next;
	}
	
	return NULL;
}