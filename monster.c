/*
 * monster.c
 *
 *  Created on: 3 dec. 2009
 *      Author: Mathieu (SPELLING *WAS* CORRECT)
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

Monster m_default[14] =
	/*      x  y  name         att fr bm iv aw pr  hp  next */
		{ { 0, 0, "Elf",       10, 0, 1, 0, 0, 10, 30, NULL },
		  { 0, 0, "Demon",     15, 0, 0, 0, 0, 4,  7,  NULL },
		  { 0, 0, "Ice golem", 10, 1, 0, 0, 0, 9,  25, NULL },
		  { 0, 0, "Ghost",     3,  0, 1, 1, 0, 9,  7,  NULL },
		  { 0, 0, "Bug",       1,  0, 0, 0, 0, 1,  1,  NULL },
		  { 0, 0, "Orc",       10, 0, 0, 0, 0, 5,  15, NULL },
		  { 0, 0, "Troll",     7,  0, 0, 0, 0, 3,  5,  NULL },
		  { 0, 0, "Black Dragon",30,1,0, 0, 0, 20, 50, NULL },
		  { 0, 0, "White Dragon",35,1,1, 0, 0, 99, 60, NULL },
		  { 0, 0, "Grue",      20, 0, 1, 0, 0, 10, 15, NULL },
		  { 0, 0, "Rat",       5,  0, 0, 0, 0, 1,  5,  NULL },
		  { 0, 0, "U-golem",   5,  0, 0, 0, 0, 1,  5,  NULL },
		  { 0, 0, "Werewolf" , 6,  0, 0, 0, 0, 3,  8,  NULL },
		  { 0, 0, "Kobold",    5,  0, 0, 0, 0, 2,  10, NULL }
		};

/* The 5 following functions are known to work correctly */

Monster * add_monster(Monster mon, int posx, int posy)
{
	Monster * current = m_list;
	Monster * new = malloc(sizeof(Monster));

	*new = mon;
	strcpy(new->name, mon.name);
	if(posx != -1)
		new->posx = posx;
	if(posy != -1)
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
		printf("Monster at %d,%d : %s with %d life, awake = %d\n", current->posx,
				current->posy, current->name, current->hp, current->awake);
		current = current->next;
	}
}
#endif

/* OK, This func isn't KNOWN to work, it's SUPPOSED to work. *
 * (however, I don't see how and where bugs could hide here) */

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

/* NOTE : it's not an error to call rm_monster on an empty
 * position.
 */
void rm_monster(int posx, int posy)
{
	Monster * current = m_list;
	Monster * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		m_list = current->next;
		free(current);
		return; // There was a bug here, killed by zale. // What bug ?
	}
	else while(current->next != NULL) {
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
/* MUST ABSOLUTELY BE CALLED AS free_monsters(m_list); */
void free_monsters(Monster * mon)
{
	if(mon == NULL) return;
	if(mon->next != NULL)
		free_monsters(mon->next);
	free(mon);

	m_list = NULL;
}

//TODO: write this func.
void m_move()
{
	Monster * current = m_list;

	while(current != NULL) {
		if(!current->awake) continue;
		/*Here, write some code that (a) finds the shortest way to the player
									 (b) returns the first step of this path. */
			//#define Dijkstra(x) ((x)*(x)*(x)+1)
		
		current = current->next;
	}
}



int m_valid(int x, int y)
{
	if(rodney.posx != x && rodney.posy != y && get_monster(x, y) == NULL &&
			(lvl_map[x][y] == T_FLOOR  ||
			 lvl_map[x][y] == T_STAIRS ||
			 lvl_map[x][y] == T_CORRIDOR))
		return 1;
	return 0;
}


/** DIRTY CODE */

void make_monsters()
{
	int nb_gen = rnd_max(2, 10);
	int posx, posy;
	int ri;


	while(nb_gen--) {
		do {
			ri = rnd_max(0, 13);
		} while (m_default[ri].proba > lvl_nb);

		do {
			posx = rnd_max(0, 10);
			posy = rnd_max(0, 21);
		} while(get_monster(posx, posy) != NULL ||
				lvl_map[posx][posy] == T_WALL ||
				lvl_map[posx][posy] == T_NONE ||
				(rodney.posx == posx && rodney.posy == posy));
		add_monster(m_default[ri], posx, posy);
	}
}
