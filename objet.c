/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 OPMORL Coding Team. All rights reserved.
 *
 */

#include "opmorl.h"
#define K 800000000


/* every object is defined w/ a x and y of -1 to be off-map. They are not to be modified, but to be copied into a custom one */
Object o_default[17] = {
	{C_WAND,    "Wand of Death",       -1, -1,  0, -80000, 0, 1,  0}, /* The Wand of Death doesn't freeze, but instakills, no arrows, one shot only (because it'd be busay)*/
	{C_WAND,    "Wand of Life",	       -1, -1,  0, 800000, 0, 1,  0}, /* same as the wand of death, more than one'd be busay */
	{C_WAND,    "Wand of Freeze",      -1, -1,  0, 0,      1, 5,  0}, /* 5 shots seem good for this one*/
	{C_WAND,    "Wand of Wounds",      -1, -1,  0, -30,    0, 10, 0}, /* nothing particular, just-a-comment*/
	{C_POTION,  "Potion of Cure",      -1, -1,  0, 15,     0, 1,  0}, /* what a shitty name man*/
	{C_POTION,  "Potion of Healing",   -1, -1,  0, 35,	   0, 1,  0}, /* the name was so awful that I changed it*/
	{C_POTION,  "Potion of Poison",    -1, -1,  0, -20,    0, 1,  0}, /* This one particularly rocks */
	{C_SWORD,   "Wooden Sword",	       -1, -1,  5, 0,	   0, K,  0}, /* Basic sword, given at the beginning of the game */
	{C_SWORD,   "Iron Sword",	       -1, -1,  7, 0,      0, K,  0}, /* This Sword is powerfuller */
	{C_SWORD,   "Steel Sword",		   -1, -1, 10, 0,      0, K,  0}, /* The most powerful sword in-game */
	{C_BOW,	    "Simple Bow",		   -1, -1,  5, 0,	   0, K,  0},
	{C_BOW,	    "Coumpound Bow",	   -1, -1, 10, 0,	   0, K,  0}, /* I do love bows */
	{C_ARROW,   "A pack of 25 Arrows", -1, -1,  0, 0,	   0, 25, 0}, /* Arrows increment from 25 the "arrows" variable from Rodney.*/
	{C_ARMOR_S, "The Shield",		   -1, -1,  0, 0,      0, K,  0},
	{C_ARMOR_B, "Leather Armor",       -1, -1,  0, 0,      0, K,  0},
	{C_ARMOR_S, "The Mithril Mail",    -1, -1,  0, 0,      0, K,  0}}; /* DONE \o/ */

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
		o_list = current->next;
		free(current);
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
/* WTF ? 123 - 456 - 789 *is* a regular pattern ! */
int find_near_free_tile(int * posx, int * posy) {
	int i, j;
	int curx = rodney.posx;
	int cury = rodney.posy;
	if(!isObject(rodney.posx, rodney.posy)) {

	}
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

	obj = NULL;
}

/* This function is to be called with an existing object at index i */
void drop_object(int i) {
	rodney.inventary[i-1]->next = rodney.inventary[i]->next;
	free(rodney.inventary[i]);
	rodney.inventary[i] = NULL;
	
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
