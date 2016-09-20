/*
 *  objet.c
 *  OPMORL
 *
 *  Created by theotime grohens on 04/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


//here's what I had done at dec. 4 23:26
#include "opmorl.h"
#include "objet.h"

Object * add_obj_list(Object obj) {
	int i;
	Object * current = obj_list;
	Object * new = malloc(sizeof(Object));

	*new = obj;
	strcpy(new->name, obj.name);

	if(obj_list == NULL) obj_list = new;
	else while(current->next != NULL)
		current = current->next;
	current->next = new;

	return new;
}

//TODO: Correct the following code

void rm_obj_list(int posx, int posy) {
	Object *temp = obj_list;
	while ((temp->next->posx != posx) && (temp->next->posy != posy)) {
		temp = temp->next;
	}
	temp->next = temp->next->next;//I think there's a problem in this func but I am not very pro in listes chainees manipulating
}
