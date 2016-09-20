/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 OPMORL Coding Team. All rights reserved.
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

/* A little feature of this func (no bug !) is that it will follow this pattern : 123 instead of : 627 
																				  456			   513
																				  789			   948 */
int find_near_free_tile(int * posx, int * posy) {
	int i, j;
	int curx = rodney.posx;
	int cury = rodney.posy;
	for (i = curx-1; i < curx+2; i++)
		for (j = cury-1; j < cury+2; j++)
			if (isObject(i, j) || lvl_map[i][j] == T_WALL || lvl_map[i][j] == T_NONE)
				return 0;
	return 1;
}
/* This function is not KNOWN to work, but where could bugs hide in 5 lines of code ? */

int isObject(int posx, int posy) {
	Object *tmp = get_object(posx, posy);
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