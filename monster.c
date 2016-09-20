/*
 * monster.c
 *
 *  Created on: 3 dec. 2009
 *      Author: Mathieu
 */

#include "opmorl.h"
#include "objet.h"

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
		  { 0, 0, "Ware-wolf", 6,  0, 0, 0, 0, 3,  8,  NULL },
		  { 0, 0, "Kobold",    5,  0, 0, 0, 0, 2,  10, NULL }
		};

/* The 5 following functions are known to work correctly */

Monster * add_monster(Monster mon, int posx, int posy)
{
	Monster * current = m_list;
	Monster * new = malloc(sizeof(Monster));

	*new = mon;
	strcpy(new->name, mon.name);
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

/* NOTE : it's not an error to call rm_monster on an unexisting
 * function. TTTHEBEST : did you mean an unexisting position ?
 * ZALE : No, i meant position containing no monster. TTTHEBEST : It's what I meant, how do you want to "call on an unexisting function" ?
 * TTTHEBEST : Yeah, lapsus. //TTTHEBEST : Why the fuck did you answer w/  MY nickname OMG
 * PLEASE NOTE : this conversation will be deleted on next+1 update.
 */
void rm_monster(int posx, int posy)
{
	Monster * current = m_list;
	Monster * tmp;

	if(current == NULL) return;

	if(current->posx == posx && current->posy == posy) {
		m_list = current->next;
		free(current);
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

	mon = NULL;
}

void m_fight()
{
	int i,j;
	for(i = rodney.posx-1; i < rodney.posx+2; i++)
		for(j = rodney.posy-1; j < rodney.posy+2; j++)
			if(get_monster(i, j) != NULL) {
				rodney.hp -= get_monster(i,j)->attack;
			}
	if (rodney.hp < 1)
		clean_exit(0);
}

void p_fight(int x, int y)
{
	Monster * mon = get_monster(x, y);
	

	if(mon == NULL) return;
	if (weapon->class == C_SWORD) mon->hp -= weapon->attack;
	else if (weapon->class == C_BOW && rodney.arrows > 1) {
		mon->hp -= weapon->attack;
		rodney.arrows--;
	}
	else /* Means there is no equipped weapon */
		mon->hp -= 3;
	mon->awake = 1; /* Wake up monster */
	if(mon->hp <= 0)
		rm_monster(x, y);
}

void zap(int x, int y, int index) /* the index of the wand in the inventory */
{
	int i;
	Monster *mon = get_monster(x, y);
	if (inventory[index]->shots_left > 0) {
		mon->hp += inventory[index]->target_hp; /* The target_hp is negative is health is removed, therefore + */
		inventory[index]->shots_left--;
	}
	if (!inventory[index]->shots_left) {
		for (i = index; i < 9; i++)
			inventory[index] = inventory[index+1];
		inventory[9] = NULL;
	}
}
int m_valid(int x, int y)
{
	if(rodney.posx != x && rodney.posy != y && get_monster(x, y) == NULL &&
			lvl_map[x][y] == T_FLOOR || lvl_map[x][y] == T_STAIRS || lvl_map[x][y] == T_CORRIDOR)
		return 1;
	return 0;
}

/* void move_monsters() //this code has been "commented" because it's error full.
{
	Monster * current = m_list;
	int d_dirx, d_diry;

	while(current != NULL) {
		if(current->awake = 0) {
			current = current->next;
			continue;
		}
		/* Slightly complex code here. We want to move towards the
		 * player in the direction that needs it the most.
		 *
		 * Please note that this code fails a whole lot : consider this case
		 *   ########
		 *   #...M..#
		 *   #.######
		 *    =====
		 *   #####.##
		 *   #...@..#
		 *   ########
		 * the monster M would try to move down unsucessfully.
		 */
		/*d_dirx = rodney.posx - current->posx;
		d_diry = rodney.posy - current->posy;
		/*                Means : check the correct cell for validity. The tern-exp is used to check the right cell.
		 *                                      vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
		/*if(abs(d_dirx) > abs(d_diry) && m_valid(current->dirx+(d_dirx<0?-1:1), current->diry)) {
			if(d_dirx < 0)
				current->dirx--;
			else
				current->dirx++;
		} else if(m_valid(current->dirx, current->diry+(d_diry<0?-1:1))) {
			if(d_diry < 0) current->diry--;
			else current->diry++;
		}
		// else do not move.
	}
} */

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
			posx = rnd_max(0, 11);
			posy = rnd_max(0, 10);
		} while(get_monster(posx, posy) != NULL ||
				lvl_map[posx][posy] == T_WALL || lvl_map[posx][posy] == T_NONE ||
				(rodney.posx == posx && rodney.posy == posy));
		add_monster(m_default[ri], posx, posy);
	}
}
