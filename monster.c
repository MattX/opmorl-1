/*
 * monster.c
 *
 *  Created on: 3 déc. 2009
 *      Author: Matthieu
 */

#include "opmorl.h"

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

/**
 * Note : this function _must_ be called as :
 * 		m_list = rm_monster(m_list,...);
 */

Monster * rm_monster(Monster * list, int posx, int posy)
{

}

void free_monsters(Monster * mon)
{
	if(mon->next != NULL)
		free_monsters(mon->next);
	free(mon);
}

void fight(Monster * mon)
{
	mon->hp -= 5;
	mon->awake = 1; /* Wake up monster */
}
