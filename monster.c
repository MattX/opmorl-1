/*
 * monster.c
 *
 *  Created on: 3 déc. 2009
 *      Author: Matthieu
 */

#include "opmorl.h"

/* The 5 following functions are known to work correctly */

Monster * add_monster(Monster mon, int posx, int posy)
{
	Monster * current = m_list;
	Monster * new = malloc(sizeof(Monster));

	*new = mon;
	if(posx > -1)
		new->posx = posx;
	if(posy > -1)
		new->posy = posy;
	new->next = NULL;

	if(current == NULL)
		return m_list = new;

	while(current->next != NULL)
		current = current->next;
	return current->next = new;
}

#ifdef DEBUG
void show_monsters()
{
	Monster * current = m_list;
	while(current != NULL) {
		printf("Monster at %d,%d : %s with %d life\n", current->posx,
				current->posy, current->name, current->hp);
		current = current->next;
	}
}
#endif

/* OK, This func isn't KNOWN to work, it's SUPPOSED to work. */
/* (however, I don't see how and where bugs could hide here) */

/** @return NULL if not found */
Monster * get_monster(int posx, int posy)
{
	Monster * current = m_list;

	while(current != NULL) {
		if(current->posx == posx && current->posy == posy)
			return current;
		current = current->next;
	}

	return NULL;
}

/* NOTE : it's not an error to call rm_monster on an unexisting
 * function.
 */
void rm_monster(int posx, int posy)
{
	Monster * current = m_list;
	Monster * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		free(current);
		m_list = current->next;
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

/* Called when a new level is generated. */
void free_monsters(Monster * mon)
{
	if(mon == NULL) return;
	if(mon->next != NULL)
		free_monsters(mon->next);
	free(mon);
}

void fight(Monster * mon)
{
	mon->hp -= 5;
	mon->awake = 1; /* Wake up monster */
}
