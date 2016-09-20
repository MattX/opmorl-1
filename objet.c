/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 OPMORL Coding Team. All rights reserved.
 *		Owner: ttthebest
 */

//TODO: write an inventory mode (see zale for details).

#include "opmorl.h"
#define K 800000000


/* every object is defined w/ a x and y of -1 to be off-map. *
 * They are not to be modified, but to be copied into a custom one */
Object o_default[17] = {
	{C_WAND,    "Wand of Death",       -1, -1,  0, -80000, 0, 1,  0}, /* The Wand of Death doesn't freeze, but instakills, no arrows, one shot only (because it'd be busay)*/
	{C_WAND,    "Wand of Life",	       -1, -1,  0, 800000, 0, 1,  0}, /* same as the wand of death, more than one'd be busay */
	{C_WAND,    "Wand of Freeze",      -1, -1,  0, 0,      1, 5,  0}, /* 5 shots seem good for this one*/
	{C_WAND,    "Wand of Wounds",      -1, -1,  0, -30,    0, 10, 0}, /* nothing particular, just-a-comment*/
	{C_POTION,  "Potion of Cure",      -1, -1,  0, 15,     0, 1,  0}, /* what a crappy name man*/ /* ZALE : NO FTW */ /* FTW ? fuck the window ? find the way ? */
	{C_POTION,  "Potion of Healing",   -1, -1,  0, 35,	   0, 1,  0}, /* the name was so awful that I changed it*/
	{C_POTION,  "Potion of Poison",    -1, -1,  0, -20,    0, 1,  0}, /* This one particularly rocks */
	{C_SWORD,   "Wooden Sword",	       -1, -1,  5, 0,	   0, K,  0}, /* Basic sword, given at the beginning of the game */
	{C_SWORD,   "Iron Sword",	       -1, -1,  7, 0,      0, K,  0}, /* This Sword is more powerful */
	{C_SWORD,   "Steel Sword",		   -1, -1, 10, 0,      0, K,  0}, /* The most powerful sword in-game */
	{C_BOW,	    "Simple Bow",		   -1, -1,  5, 0,	   0, K,  0},
	{C_BOW,	    "Coumpound Bow",	   -1, -1, 10, 0,	   0, K,  0}, /* I do love bows */
	{C_ARROW,   "A pack of 25 Arrows", -1, -1,  0, 0,	   0, 25, 0}, /* Arrows increment from 25 the "arrows" variable from Rodney.*/
	{C_ARMOR_S, "The Shield",		   -1, -1,  0, 0,      0, K,  0},
	{C_ARMOR_B, "Leather Armor",       -1, -1,  0, 0,      0, K,  0},
	{C_ARMOR_S, "The Mithril Mail",    -1, -1,  0, 0,      0, K,  0}}; /* DONE \o/ */


Object * add_object(Object obj, int posx, int posy)
{
	Object * current = o_list;
	Object * new = malloc(sizeof(Object));

	*new = obj;
	strcpy(new->name, obj.name);
	if(posx != -1)
		new->posx = posx;
	if(posy != -1)
		new->posy = posy;
	new->next = NULL;

	if(current == NULL)
		return o_list = new;

	while(current->next != NULL)
		current = current->next;

	return current->next = new;
}

void rm_object(int posx, int posy)
{
	Object * current = o_list;
	Object * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		o_list = current->next;
		free(current);
		return; //see rm_monster in monster.c
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

#ifdef DEBUG
void show_objects()
{
	Object * current = o_list;
	while(current != NULL) {
		printf("Object at %d,%d : %s.\n", current->posx,
				current->posy, current->name);
		current = current->next;
	}
}
#endif


int find_near_free_tile(int * posx, int * posy) {
	int i, j;
	int x = rodney.posx, y = rodney.posy;

	if (!isObject(x, y)) {
		*posx = x; *posy = y;
		return 1;
	} else {
		for (i = x-1; i < x+2; i++)
			for (j = y-1; j < y+2; j++)
				if (!isObject(i, j) && lvl_map[i][j] != T_WALL 
					&& lvl_map[i][j] != T_NONE
					&& lvl_map[i][j] != T_STAIRS) { //TODO: Can we drop in stairs ?
					*posx = i;
					*posy = j;
					return 1;
				}
	}
	return 0; // This should be reached when dropping fails, which means the player is surrounded by objects
}
/* This function is not KNOWN to work, but where could bugs hide in 5 lines of code ? */

int isObject(int posx, int posy) {
	Object *tmp = get_object(posx, posy);
	if (tmp == NULL)
		return 0;
	//There was a bug here.
	return 1;
}


/* Called when a new level is generated. */
/* MUST ABSOLUTELY BE CALLED AS free_objects(o_list); */
void free_objects(Object * obj)
{
	if(obj == NULL) return;
	if(obj->next != NULL)
		free_objects(obj->next);
	free(obj);

	o_list = NULL;
}

/* This function is to be called with an existing object at index i */
int drop_object(int i) {
	int j;

	if(i < 0 || i > 13) return 1;

	if (i < 10) { /* means the object is in the inventory */
		if (inventory[i] == NULL) return 1;
		if (find_near_free_tile(&(inventory[i]->posx), &(inventory[i]->posy))) {
			add_object(*inventory[i], -1, -1);
			free(inventory[i]);
			inventory[i] = NULL;

			for (j = i+1; j < 10; j++)
				inventory[j-1] = inventory[j]; //this should _not_ segfault
			inventory[9] = NULL;

			return EPIC_WIN; // dropping succeeded
		}
		return EPIC_FAIL; // FAIL
	}

	else { /* the object is either the equipped weapon, armor or shield */
		if (i == WEAPON_SLOT && weapon != NULL) {
			if (find_near_free_tile(&weapon->posx, &weapon->posy)) {
				add_object(*weapon, -1, -1);
				free(weapon);
				weapon = NULL;
				return EPIC_WIN;
			}
		}

		else if (i == ARMOR_SLOT && armor != NULL) {
			if (find_near_free_tile(&armor->posx, &armor->posy)) {
				add_object(*armor, -1, -1);
				free(armor);
				armor = NULL;
				return EPIC_WIN;
			}
		}

		else if (i == SHIELD_SLOT && shield != NULL) {
			if (find_near_free_tile(&shield->posx, &shield->posy)) {
				add_object(*shield, -1, -1);
				free(shield);
				shield = NULL;
				return EPIC_WIN;
			}
		}
	}

	return EPIC_FAIL;
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

void getObject() {
	int i = -1;
	while(inventory[++i] != NULL);
	if(i >= 10) {
		printf("Sorry, your inventory is full. You should try dropping something, or dying\n");
		return;
	}

	inventory[i] = malloc(sizeof(Object));
	*inventory[i] = *get_object(rodney.posx, rodney.posy);
	rm_object(rodney.posx, rodney.posy);
}

void equip(int inv_index)
{
	Object * tmp;
	int slot;

	if(inv_index < 0 || inv_index > 9 || inventory[inv_index] == NULL) return;
	
	switch (inventory[inv_index]->class) {
		case C_SWORD:
		case C_BOW:
			slot = WEAPON_SLOT;
			break;
		case C_ARMOR_B:
			slot = ARMOR_SLOT;
			break;
		case C_ARMOR_S:
			slot = SHIELD_SLOT;
			break;
	}
	
	switch(slot) {
	case WEAPON_SLOT:
		if(inventory[inv_index]->class != C_BOW || inventory[inv_index]->class
				!= C_SWORD) return;
		tmp = weapon;
		weapon = inventory[inv_index];
		break;
	case ARMOR_SLOT:
		if(inventory[inv_index]->class != C_ARMOR_B) return;
		tmp = armor;
		armor = inventory[inv_index];
		break;
	case SHIELD_SLOT:
		if(inventory[inv_index]->class != C_ARMOR_S) return;
		tmp = shield;
		shield = inventory[inv_index];
		break;
	}
	
	inventory[inv_index] = tmp;
}

void wish()
{
	int ind;

	printf("What do you wish for (id) ? ");
	scanf("%d", &ind);

	if(ind > 16) return;

	//rm_object(rodney.posx, rodney.posy);
	add_object(o_default[ind], rodney.posx, rodney.posy);
}

/** DIRTY CODE */

void make_objects()
{
	int nb_gen = rnd_max(2, 10);
	int posx, posy;
	int ri; // ri for remind, idiot ! 

	while(nb_gen--) {
		ri = rnd_max(0, 16);

		printf("Selected object %d for creation, name %s.\n", ri, o_default[ri].name);
		do {
			posx = rnd_max(0, 10);
			posy = rnd_max(0, 21);
		} while(get_object(posx, posy) != NULL ||
				lvl_map[posx][posy] == T_WALL || lvl_map[posx][posy] == T_NONE ||
				(rodney.posx == posx && rodney.posy == posy));
		add_object(o_default[ri], posx, posy);
	}
}

/** DIRTY CODE ENDS HERE, FOR GOD'S SAKE */

void zap(int x, int y, int index) /* the index of the wand in the inventory */
{
	int i;
	Monster *mon = get_monster(x, y);
	if (mon == NULL) return;
	if (inventory[index]->shots_left > 0) {
		mon->hp += inventory[index]->target_hp; /* The target_hp is negative -> health is removed, therefore + */
		inventory[index]->shots_left--;
	}
	if (!inventory[index]->shots_left) {
		for (i = index; i < 9; i++)
			inventory[index] = inventory[index+1];
		inventory[9] = NULL;
	}
}

void zap_display() {
	int i, is, wand, x, y;
	for (i = 0; i < 10; i++)
		if (inventory[i]->class == C_WAND) {
			printf("%d. %s with %d shot(s) left.", i, inventory[i]->name, inventory[i]->shots_left);
			is++;
		}
	if (is) {
		printf("Which wand to use ?");
		scanf("%d", &wand);
		if (inventory[wand]->class != C_WAND)  {
			printf("This is not a wand.\n");
			return;
		}
		printf("Which coordonates to zap ?\n");
		scanf("%d%d", &x, &y);
		zap(x, y, wand); //The zap func should check whether the coordonates are valid.
	}
	else
		printf("There is no wand to use.\n");
	return;
	
}

void equip_display() {
	int i, index;
	for (i = 0; i < 10; i++) 
		if (inventory[i])
			if (inventory[i]->class == C_ARMOR_B ||
				inventory[i]->class == C_ARMOR_S ||
				inventory[i]->class == C_BOW	 ||
				inventory[i]->class == C_SWORD)
				printf("%d. A %s\n", i, inventory[i]->name);
	scanf("%d", &index);
	equip(index); //Seems not functional.
}

void drink() {
	int i, is = 0, potion;
	for (i = 0; i < 10; i++)
		if (inventory[i]->class == C_WAND) {
			printf("%d. A %s", i, inventory[i]->name);
			is++;
		}
	if (!is) {
		printf("You don't have any potion.");
		return;
	}
	else {
		printf("Which one do you want to drink ?\n");
		scanf("%d", &potion);
	}
	if (inventory[potion]->class != C_POTION) {
		printf("This is no potion, sucker.\n");
		return;
	}
	rodney.hp+= inventory[potion]->target_hp;
	free(inventory[potion]);
	for (i = potion+1; i < 10; i++)
		inventory[i-1] = inventory[i];
	inventory[9] = NULL;

}
