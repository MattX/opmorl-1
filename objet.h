/*
 *  objet.h
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

typedef enum {
	C_WAND, C_POTION, C_BOW, C_SWORD, C_ARROW, C_ARMOR_B, C_ARMOR_S
} Class; /* ARMOR_B : body ; ARMOR_S : shield */

typedef struct obj_type {
	Class class;
typedef struct obj_type { Class class;
	char name[50];
	int posx, posy; 
	int attack; /* For weapons */ int target_hp; /* For wands & potions, bonus of life (if > 0) or damage (if < 0) they do to the target */
	int freezes; /* For wands */
	int shots_left; /* For wands */ /* When shots_left == 0 the wand is deleted */
	int nb_arrows; /* For arrows */ /* Note on arrows : they are always generated as a single object containing 25
									 * arrows. At this point, nb_arrows == 25. Every time a shot is made, nb_arrows
									 * is decremented and when nb_arrows == 0 the object is deleted */
	struct obj_type * next; } Object;

Object obj_default[16];
Object inv[10];
Object weapon_slot;
Object armour_b_slot;
Object armour_s_slot;
Object * obj_list;

Object add_object(Object obj); /* Alloc room for a new object in obj_list,
								  * then copy the obj parameter into it. */
/* @return a pointer on the new object */
int isSomething(int posx, int posy);
void rm_object(int posx, int posy); /* Remove the object at posx, posy */
void free_objects(); /* free() all the list */
int find_near_free_tile(int * posx, int * posy);
/* Trouve la case libre la plus proche de *posx, *posy (celle-ci comprise)
 *  et met ses coordonneees dans *posx, *posy */
/* @return 1 si objet droppe, 0 sinon*/
