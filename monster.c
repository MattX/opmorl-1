/*
 * monster.c
 *
 *  Created on: 3 dec. 2009
 *      Author: Mathieu (SPELLING *WAS* CORRECT)
 *	Copyright : Opmorl, inc.
 */

#include "opmorl.h"

Monster m_default[14] =
	/*      x  y  name         att fr bm iv aw min max hp  next  org */
		{ { 0, 0, "Elf",       10, 0, 1, 0, 0, 12, 25, 30, NULL, 0 },
		  { 0, 0, "Demon",     15, 0, 0, 0, 0, 4,  15, 7,  NULL, 1 },
		  { 0, 0, "Ice golem", 10, 1, 0, 0, 0, 9,  20, 25, NULL, 2 },
		  { 0, 0, "Ghost",     3,  0, 1, 1, 0, 9,  18, 7,  NULL, 3 },
		  { 0, 0, "Bug",       1,  0, 0, 0, 0, 1,  3,  1,  NULL, 4 },
		  { 0, 0, "Orc",       10, 0, 0, 0, 0, 5,  15, 15, NULL, 5 },
		  { 0, 0, "Troll",     7,  0, 0, 0, 0, 3,  13, 5,  NULL, 6 },
		  { 0, 0, "Black Dragon",30,1,0, 0, 0, 18, 25, 50, NULL, 7 },
		  { 0, 0, "White Dragon",35,1,1, 0, 0, 21, 25, 60, NULL, 8 },
		  { 0, 0, "Grue",      20, 0, 1, 1, 0, 10, 25, 15, NULL, 9 },
		  { 0, 0, "Rat",       5,  0, 0, 0, 0, 1,  5,  5,  NULL, 10 },
		  { 0, 0, "U-golem",   3,  0, 0, 0, 0, 1,  5,  8,  NULL, 11 },
		  { 0, 0, "Werewolf" , 6,  0, 0, 0, 0, 3,  14, 8,  NULL, 12 },
		  { 0, 0, "Kobold",    5,  0, 0, 0, 0, 2,  12, 10, NULL, 13 }
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

void m_move()
{
	Monster * current = m_list;

	while(current != NULL) {
		if(!current->awake) {
			current = current->next;
			continue;
		}
		if(current->orig == 1000) {
			y_ai();
			current = current->next;
			continue;
		}
		mon_move(current->posx, current->posy, 1);
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

/** fmonat - find monster at.
 * @return a pointer on the monster.
 * @return NULL if no monster found, rod may be set to 1 if rodney found.*/

Monster * fmonat(int x0, int y0, int x1, int y1, int * rod)
{
	int steep = abs(y1 - y0) > abs(x1 - x0);
	int deltax, deltay, error;
	int y, ystep;
	int x;
	Monster * mon;

	if(steep) {
		swap(&y0, &x0);
		swap(&y1, &x1);
	}

	if(x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	deltax = x1-x0;
	deltay = abs(y1 - y0);
	error = deltax/2;
	y = y0;

	if(y0 < y1) ystep = 1;
	else ystep = -1;

	for(x = x0; x <= x1; x++) {
		if(steep) {
			if((mon = get_monster(y, x)))
				return mon;
			if(y == rodney.posx && x == rodney.posy && x != x0 && y != y0) {// SIC.
				*rod = 1;
				return NULL;
			}
		} else {
			if((mon = get_monster(x, y)))
				return mon;
			if(x == rodney.posx && y == rodney.posy && x != x0 && y != y0) {
				*rod = 1;
				return NULL;
			}
		}

		error -= deltay;
		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}

	return NULL;
}

#ifdef DEBUG
void add_rat()
{
	int x, y;

	printf("Where ? ");
	scanf("%d%d", &x, &y);

	rm_monster(x, y);
	add_monster(m_default[10], x, y);
}
#endif


int count_valid_monsters()
{
	int i;
	int sum = 0;

	for(i = 0; i < 14; i++)
		if(m_default[i].lmin <= lvl_nb && m_default[i].lmax >= lvl_nb) sum++;

	return sum;
}

int val_monster_at(int index)
{
	int i;

	for(i = 0; i < 14; i++) {
		if(m_default[i].lmin <= lvl_nb && m_default[i].lmax >= lvl_nb)
			index--;
		if(index == 0)
			return i;
	}

	fprintf(stderr, "valmonat error\n");
	return 0;
}

//TODO: continue fixing

/** DIRTY CODE */
void make_monsters()
{
	int nb_gen = rnd_max(2, 10);
	int posx, posy;
	int ri;


	while(nb_gen--) {
		ri = rnd_max(0, count_valid_monsters());

		do {
			posx = rnd_max(0, 10);
			posy = rnd_max(0, 21);
		} while(get_monster(posx, posy) != NULL ||
				lvl_map[posx][posy] == T_WALL ||
				lvl_map[posx][posy] == T_NONE ||
				(rodney.posx == posx && rodney.posy == posy));
		add_monster(m_default[val_monster_at(ri)], posx, posy);
	}
}

Monster * find_yendor()
{
	Monster * current = m_list;

	while(current != NULL) {
		if(current->orig == 1000)
			return current;
		current = current->next;
	}

	return NULL;
}
