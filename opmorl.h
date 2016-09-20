/*
 * opmorl.h
 *
 *  Created on: 2 dec. 2009
 *      Author: zale&ttthebest
 */

#ifndef OPMORL_H_
#define OPMORL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define FINAL_LVL 25
#define DEBUG 1
#ifndef DEBUG
#define DEBUG 0
#endif
#define K 80000000

typedef enum { T_FLOOR, T_WALL, T_CORRIDOR, T_NONE, T_STAIRS } Tile;
Tile lvl_map[12][11];
typedef enum { TS_SEEN, TS_DARK, TS_UNVISITED } Tilestat;
Tilestat map_status[12][11];

int going_up;
int lvl_nb;
int turn;

typedef struct {
	int posx, posy;
	int gold;
	int sword_b, bow_b;
	int exp_b;
	int hp;
} Player;

Player rodney;

typedef struct m_type {
	int posx, posy;
	char name[50];
	int attack;
	int freezes, blocks_magic;
	int is_invisible;
	int awake;
	int proba;
	int hp;

	struct m_type * next;
} Monster;

extern Monster m_default[14];
Monster * m_list;

typedef enum {
	C_WAND, C_POTION, C_BOW, C_SWORD, C_ARROW, C_ARMOR_B, C_ARMOR_S
} Class; /* ARMOR_B : body ; ARMOR_S : shield */

typedef struct obj_type { 
	Class class; 
	char name[50];
	int posx, posy; 
	int attack; /* For weapons */
	int target_hp; /* For wands & potions, bonus of life (if > 0) or damage (if < 0) they do to the target */
	int freezes; /* For wands */
	int shots_left; /* For wands */
	/* When shots_left == 0 the wand is deleted */
	int nb_arrows; /* For arrows */
	/* Note on arrows : they are always generated as a single object containing 25
	 * arrows. At this point, nb_arrows == 25. Every time a shot is made, nb_arrows
	 * is decremented and when nb_arrows == 0 the object is deleted */
	struct obj_type * next; 
} Object;
 

/* PLEASE NOTE : for ANSI C compat reasons,
 * o_default and m_default must be decared as :
 *     extern Object o_default[17];
 * in opmorl.h and
 *     Object o_default[17] = { ... };
 * in objet.c.
 */
/* ALSO NOTE : STOP WITH CRAPPY VAULES, SHIT ! Unused values must be set to 0,
 * not K, -K or any random number. Please correct this. */

extern Object o_default[17];

Object * o_list; 

/* FUNCTIONS */

void fill_map();
void display_map();
void display_msg();
void first_init();
void move_letter(char c);
void fight(int x, int y);
Monster * get_monster(int posx, int posy);
Monster * add_monster(Monster mon, int posx, int posy);
void free_monsters(Monster * mon);
void make_monsters();
void free_objects(Object *);

int min(int,int);
int max(int,int);
int rnd_max(int,int);

void check_visit();

#include "objet.h"

#endif /* OPMORL_H_ */
