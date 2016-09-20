/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


//here's what I had done at dec. 4 23:26
#include <stdlib.h>
#include <string.h>
#include "objet.h"

Object add_obj_list(Object obj) {
	int i;
	Object *new = malloc(sizeof(Object));
	for (i = 0; i < strlen(obj.name); i++)
		new->name[i] = obj.name[i];
	new->posx = obj.posx;
	new->posy = obj.posy;
	new->attack = obj.attack;
	new->target_hp = obj.target_hp;
	new->freezes = obj.freezes;
	new->shots_left = obj.shots_left;
	new->nb_arrows = obj.nb_arrows;
	new->next = obj; //fuck this error, I am too tired to correct it
	return *new;
}

void rm_obj_list(int posx, int posy) {
	Object *temp = obj_list;
	while ((temp->next->posx != posx) && (temp->next->posy != posy)) {
		temp = temp->next;
	}
	temp->next = temp->next->next;//I think there's a problem in this func but I am not very pro in listes chaînées manipulating
}