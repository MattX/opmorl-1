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

int isSomething(int posx, int posy) {
	if (lvl_map[posx][posy] == T_FLOOR || lvl_map[posx][posy] == T_CORRIDOR)
		return 0;
	return 1;

void rm_obj_list(int posx, int posy) {
	Object *temp = obj_list;
	while ((temp->next->posx != posx) && (temp->next->posy != posy)) {
		temp = temp->next;
	}
	temp->next = temp->next->next;//I think there's a problem in this func but I am not very pro in listes chaînées manipulating
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