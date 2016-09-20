/*
 *  objet.h
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "opmorl.h"

/* Same as Object, see below */


Object obj_default[16];
Object inv[10];
Object weapon_slot;
Object armour_b_slot;
Object armour_s_slot;
Object * obj_list;

/* The definition of Object type has been moved to "opmorl.h", for compatibility reasons */


Object add_object(Object obj); /* Alloc room for a new object in obj_list,
								  * then copy the obj parameter into it. */
/* @return a pointer on the new object */
Object * get_object(int posx, int posy);
int isSomething(int posx, int posy);
/* NOTNEEDED */
int isObject(int posx, int posy);
int isMonster(int posx, int posy);
/* ENDNOTNEEDED */
void rm_object(int posx, int posy); /* Remove the object at posx, posy */
void free_objects(); /* free() all the list */
int find_near_free_tile(int * posx, int * posy);
/* Trouve la case libre la plus proche de *posx, *posy (celle-ci comprise)
 *  et met ses coordonneees dans *posx, *posy */
/* @return 1 si objet droppe, 0 sinon*/
