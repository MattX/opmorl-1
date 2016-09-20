/*
 *  objet.h
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef OBJET_H_
#define OBJET_H_
#include "opmorl.h"

/* The definition of Object type has been moved to "opmorl.h", for compatibility reasons */


Object * add_object(Object obj, int posx, int posy);
/* Alloc room for a new object in obj_list, then copy the obj parameter into it. */
/* @return a pointer on the new object */
Object * get_object(int posx, int posy);
void getObject(); /* this function, not to be confused with get_object, gets the object at rodney.posx rodney.posy and puts it in the inventary. */
int isObject(int posx, int posy);
void rm_object(int posx, int posy); /* Remove the object at posx, posy */
void free_objects(); /* free() all the list */
int drop_object(int i);
void drop(); /* This func, defined in player.c, displays the inventory and asks the player which object to drop. It calls then drop_object and says if whether the drop was successful */
int find_near_free_tile(int * posx, int * posy);
void zap(int x, int y, int index);
void zap_display();
void equip_display();
void drink();
/* Trouve la case libre la plus proche de *posx, *posy (celle-ci comprise)
 *  et met ses coordonneees dans *posx, *posy */
/* @return 0 si objet droppe, 1 sinon*/
void wish();
void make_objects();


#endif
